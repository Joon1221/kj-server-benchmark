//==============================================================================
// Source: https://blog.csdn.net/RichardYSteven/article/details/50559503
//==============================================================================

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <errno.h>
#include <sys/ioctl.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#include <thread>

#define VIDEO_REQUEST 0
#define CHAT_REQUEST 1

#define MAXEVENTS 64

static int socket_fd, epoll_fd;

RSA *createRSAWithFilename(char * filename, bool publicKey);
void sendVideo(int fd);
void sendChatMsg(int fd);

//------------------------------------------------------------------------------
// Source: http://hayageek.com/rsa-encryption-decryption-openssl-c/
//------------------------------------------------------------------------------

RSA *createRSAWithFilename(char * filename, bool publicKey) {
    FILE * fp = fopen(filename,"rb");
    
    if(fp == NULL) {
        printf("Unable to open file %s \n",filename);
        return NULL;
    }
    RSA *rsa = RSA_new();
    
    if(publicKey)  {
        rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
    }
    else {
        rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
    }
    
    return rsa;
}

RSA* rsaEncrypt = createRSAWithFilename("public.pem", true);
RSA* rsaDecrypt = createRSAWithFilename("private.pem", false);

struct videoStreamingPacket {
    // custom made protocol
    uint8_t       type;
    uint8_t       body_length[3]; /* lenght of the body */
    uint8_t       stream_id[3]; /* reserved, must be "\0\0\0" */
    uint8_t       body[192]; /* data body */
};

struct chatPacket {
    // custom made protocol based off websocket
    uint8_t       header;
    uint8_t       body_length[3]; /* lenght of the body */
    uint8_t       chat_id[3]; /* reserved, must be "\0\0\0" */
    uint8_t       body[192]; /* data body */
};

//==============================================================================
// uint8_t       type;
// uint8_t       body_length[3]; /* lenght of the body */
// uint8_t       stream_id[3]; /* reserved, must be "\0\0\0" */
// uint8_t       body[192]; /* data body */
//==============================================================================

void sendVideo(int fd) {
    int count = 0;
    
    FILE *fp;
    fp = fopen("../shared/1280x720.mp4", "rb");
    
    if(fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    
    videoStreamingPacket packet;
    
    int numCharRead = fread(&packet.body, 1, 192, fp);
    
    while (true) {
        // break if the full 192 bytes are not read
        if (numCharRead != 192) {
            break;
        }
        
        packet.type = 0x12; // use static value
        packet.stream_id[0] = 0x12345678; // use static value
        packet.body_length[0] = 192;
        numCharRead = fread(&packet.body, 1, 192, fp);
        
        //------------------------------------------------------------------
        // Encrypt Message
        //------------------------------------------------------------------
        char msgEncrypted[256];    // Encrypted message
        
        //   RSA *rsaEncrypt = createRSAWithFilename("public.pem", true);
        
        
        // Encrypt the message
        char *err = (char *)malloc(130);
        if((RSA_public_encrypt(199, (unsigned char*)&packet, (unsigned char*)msgEncrypted,
                               rsaEncrypt, RSA_PKCS1_OAEP_PADDING)) == -1) {
            ERR_load_crypto_strings();
            ERR_error_string(ERR_get_error(), err);
            fprintf(stderr, "Error encrypting message: %s\n", err);
        }
        
        //        printf("Encrypted message: %s\n", msgEncrypted);
        
        //------------------------------------------------------
        // Send Message
        //------------------------------------------------------
        //        size_t error = write(fd, &msgEncrypted, 1);
        
        size_t error = write(fd, &msgEncrypted, 256);
        ////        error = write(f, message, strlen(message));
        if (error < 0) {
            printf("error");
            perror("write");
            exit(1);
        }
        //        printf("Fd: %d Count: %d\n", fd, count);
    }
    
    //    videoStreamingPacket packet;
    
    packet.type = 0x12; // use static value
    packet.stream_id[0] = 0x12345678; // use static value
    packet.body_length[0] = numCharRead;
    
    fread(&packet.body, 1, numCharRead, fp); // read the remaining data
    
    //------------------------------------------------------------------
    // Encrypt Message
    //------------------------------------------------------------------
    char msgEncrypted[256];    // Encrypted message
    
    //   RSA *rsaEncrypt = createRSAWithFilename("public.pem", true);
    
    
    // Encrypt the message
    char *err = (char *)malloc(130);
    if((RSA_public_encrypt(199, (unsigned char*)&packet, (unsigned char*)msgEncrypted,
                           rsaEncrypt, RSA_PKCS1_OAEP_PADDING)) == -1) {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error encrypting message: %s\n", err);
    }
    
    //    printf("Encrypted message: %s\n", msgEncrypted);
    
    //------------------------------------------------------
    // Send Message
    //------------------------------------------------------
    write(fd, &msgEncrypted, 256);
    fclose(fp);
}

//==============================================================================
// uint8_t       header;
// uint8_t       body_length[3]; /* lenght of the body */
// uint8_t       chat_id[3]; /* reserved, must be "\0\0\0" */
// uint8_t       body[192]; /* data body */
//==============================================================================

void sendChatMsg(int fd) {
    FILE *fp;
    fp = fopen("../shared/chat_message.txt", "rb");
    
    if(fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }
    
    chatPacket packet;
    
    packet.header = 0x12; // use static value
    int randNum = rand() % 192;
    packet.body_length[0] = randNum; // use a random value so that all packets are not identical
    packet.chat_id[0] = 0x12345678; // use static value
    int numCharRead = fread(&packet.body, 1, randNum, fp);
    
//    printf("randNum: %d\n", randNum);
//    printf("packet.body: %s\n", packet.body);
    //    int randSize = rand() % 192;
    
    //------------------------------------------------------------------
    // Encrypt Message
    //------------------------------------------------------------------
    char msgEncrypted[256];    // Encrypted message
    
    // Encrypt the message
    char *err = (char *)malloc(130);
    if((RSA_public_encrypt(7+randNum, (unsigned char*)&packet, (unsigned char*)msgEncrypted,
                           rsaEncrypt, RSA_PKCS1_OAEP_PADDING)) == -1) {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error encrypting message: %s\n", err);
    }
    
    //            printf("Encrypted message: %s\n", msgEncrypted);
    
    //------------------------------------------------------
    // Send Message
    //------------------------------------------------------
    //        size_t error = write(fd, &msgEncrypted, 1);
    
    size_t error = write(fd, &msgEncrypted, 256);
    if (error < 0) {
        printf("error");
        perror("write");
        exit(1);
    }
    fclose(fp);
}

//-----------------------------------------------------------------------------
// Open Socket
//-----------------------------------------------------------------------------

static void socket_create_bind_local() {
    struct sockaddr_in server_addr;
    int opt = 1;
    
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
    
    if (setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int)) == -1) {
        perror("Setsockopt");
        exit(1);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(7777);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero),8);
    
    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
        == -1) {
        perror("Unable to bind");
        exit(1);
    }
    
}

//-----------------------------------------------------------------------------
// Make Server Asynchronous
//-----------------------------------------------------------------------------

static int make_socket_non_blocking(int sfd) {
    int flags;
    
    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        return -1;
    }
    
    flags |= O_NONBLOCK;
    if (fcntl(sfd, F_SETFL, flags) == -1) {
        perror("fcntl");
        return -1;
    }
    
    return 0;
}

//-----------------------------------------------------------------------------
// Accept New Connections
//-----------------------------------------------------------------------------

void accept_and_add_new() {
    struct epoll_event event;
    struct sockaddr in_addr;
    socklen_t in_len = sizeof(in_addr);
    int infd;
    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
    
    while ((infd = accept(socket_fd, &in_addr, &in_len)) != -1) {
        
        if (getnameinfo(&in_addr, in_len,
                        hbuf, sizeof(hbuf),
                        sbuf, sizeof(sbuf),
                        NI_NUMERICHOST | NI_NUMERICHOST) == 0) {
            printf("Accepted connection on descriptor %d (host=%s, port=%s)\n",
                   infd, hbuf, sbuf);
        }
        /* Make the incoming socket non-block
         * and add it to list of fds to
         * monitor*/
        if (make_socket_non_blocking(infd) == -1) {
            abort();
        }
        
        event.data.fd = infd;
        event.events = EPOLLIN | EPOLLET;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, infd, &event) == -1) {
            perror("epoll_ctl");
            abort();
        }
        in_len = sizeof(in_addr);
    }
    
    if (errno != EAGAIN && errno != EWOULDBLOCK)
        perror("accept");
    /* else
     *
     * We hae processed all incomming connectioins
     *
     */
}

//-----------------------------------------------------------------------------
// Receive Data
//-----------------------------------------------------------------------------

void process_new_data(int fd) {
    ssize_t count;
    char msgFromClient[256];
    
    while ((count = read(fd, msgFromClient, 256))) {
        if (count == -1) {
            /* EAGAIN, read all data */
            if (errno == EAGAIN)
                return;
            
            perror("read");
            break;
        }
        
        /* Write buffer to stdout */
        msgFromClient[count] = '\0';
//        printf("Message received from client: %s \n", msgFromClient);
  
        //------------------------------------------------------
        // Decrypt Message
        //------------------------------------------------------
        int requestType;    // Decrypted message
        char *err;               // Buffer for any error messages

        // Decrypt it
        if(RSA_private_decrypt(256, (unsigned char*)msgFromClient, (unsigned char*)&requestType,
                               rsaDecrypt, RSA_PKCS1_OAEP_PADDING) == -1) {
            ERR_load_crypto_strings();
            ERR_error_string(ERR_get_error(), err);
            fprintf(stderr, "Error decrypting message: %s\n", err);
        }
        printf("Request Type: %d\n", requestType);

        if (requestType == VIDEO_REQUEST) {
            printf("Video Request\n");
            sendVideo(fd);
        }
        else if (requestType == CHAT_REQUEST) {
            printf("Chat Request\n");
            sendChatMsg(fd);
        }
        
//        //------------------------------------------------------
//        // Decrypt Message
//        //------------------------------------------------------
//        char msgDecrypted[256];    // Decrypted message
//        char *err;               // Buffer for any error messages
//
//        // Decrypt it
//        if(RSA_private_decrypt(256, (unsigned char*)msgFromClient, (unsigned char*)msgDecrypted,
//                               rsaDecrypt, RSA_PKCS1_OAEP_PADDING) == -1) {
//            ERR_load_crypto_strings();
//            ERR_error_string(ERR_get_error(), err);
//            fprintf(stderr, "Error decrypting message: %s\n", err);
//        }
//        printf("Decrypted message: %d\n", msgDecrypted[0]);
        
//        //------------------------------------------------------------------
//        // Encrypt Message
//        //------------------------------------------------------------------
//        char msg[256];  // Message to encrypt
//        msg[0] = 'h';
//        msg[1] = 'e';
//        msg[2] = 'l';
//        msg[3] = 'l';
//        msg[4] = 'o';
//        msg[5] = '\0';
//        char msgEncrypted[256];    // Encrypted message
//
//        // Encrypt the message
//        err = (char *)malloc(130);
//        if((RSA_public_encrypt(strlen(msg)+1, (unsigned char*)msg, (unsigned char*)msgEncrypted,
//                               rsaEncrypt, RSA_PKCS1_OAEP_PADDING)) == -1) {
//            ERR_load_crypto_strings();
//            ERR_error_string(ERR_get_error(), err);
//            fprintf(stderr, "Error encrypting message: %s\n", err);
//        }
//
//        //------------------------------------------------------
//        // Send Message
//        //------------------------------------------------------
//        write(fd, &msgEncrypted, 256);
    }
    printf("Close connection on descriptor: %d\n", fd);
    close(fd);
}

//-----------------------------------------------------------------------------
// Initialize Program & Run Main Loop
//-----------------------------------------------------------------------------

int main() {
    struct epoll_event event, *events;
    
    socket_create_bind_local();
    
    if (make_socket_non_blocking(socket_fd) == -1)
        exit(1);
    
    if (listen(socket_fd, 5) == -1) {
        perror("Listen");
        exit(1);
    }
    
    printf("\nTCPServer Waiting for client on port 7777\n");
    fflush(stdout);
    
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(1);
    }
    
    event.data.fd = socket_fd;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) == -1) {
        perror("epoll_ctl");
        exit(1);
    }
    
    events = (epoll_event *)calloc(MAXEVENTS, sizeof(event));
    
    while(1) {
        int n, i;
        n = epoll_wait(epoll_fd, events, MAXEVENTS, -1);
        for (i = 0; i < n; i++) {
            if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP ||
                !(events[i].events & EPOLLIN)) {
                /* An error on this fd or socket not ready */
                perror("epoll error");
                close(events[i].data.fd);
            } else if (events[i].data.fd == socket_fd) {
                /* New incoming connection */
//                printf("New Connection\n");
                accept_and_add_new();
            } else {
                /* Data incoming on fd */
//                printf("Data Incoming\n");
                //process_new_data(events[i].data.fd);
                std::thread{process_new_data, (int)events[i].data.fd}.detach();

            }
        }
    }
    
    free(events);
    close(socket_fd);
    return 0;
}

// The error below can be ignoreed.
/*
epoll error: Resource temporarily unavailable
read: Bad file descriptor
Close connection on descriptor: 7
read: Bad file descriptor
Close connection on descriptor: 7
 */

