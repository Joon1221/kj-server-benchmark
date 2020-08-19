/*  For our final example, server5.c,
 we include the sys/time.h and sys/ioctl.h headers in place of signal.h
 in our last program and declare some extra variables to deal with select.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>

#include <thread>

#define VIDEO_REQUEST 0
#define CHAT_REQUEST 1

RSA *createRSAWithFilename(char * filename, bool publicKey);
void handleRequest(int fd, char msgFromClient[]);
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

RSA *rsaEncrypt = createRSAWithFilename("public.pem", true);
RSA *rsaDecrypt = createRSAWithFilename("private.pem", false);

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



void handleRequest(int fd, char msgFromClient[]) {
    printf("Message Received from %d\n", fd);
    
    //------------------------------------------------------
    // Decrypt Message
    //------------------------------------------------------
    int requestType;    // Decrypted message
    char *err;               // Buffer for any error messages

    // Decrypt it
    if(RSA_private_decrypt(256, (unsigned char*)msgFromClient, (unsigned char *)&requestType,
                           rsaDecrypt, RSA_PKCS1_OAEP_PADDING) == -1) {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error decrypting message: %s\n id: %d\n", err, fd);
        printf("Request Type: %d\n", requestType);
    }
    
//    printf("Request Type: %d\n", requestType);
    
    if (requestType == VIDEO_REQUEST) {
//        printf("Video Request\n");
        sendVideo(fd);
    }
    else if (requestType == CHAT_REQUEST) {
//        printf("Chat Request\n");
        sendChatMsg(fd);
    }
}

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
        count++;
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

int main() {
//    sendVideo(1);
    
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int result;
    fd_set readfds, testfds;
    
    /*  Create and name a socket for the server.  */
    
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8888);
    server_len = sizeof(server_address);
    
    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
    
    /*  Create a connection queue and initialize readfds to handle input from server_sockfd.  */
    
    listen(server_sockfd, 5);
    
    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);
    
    /*  Now wait for clients and requests.
     Since we have passed a null pointer as the timeout parameter, no timeout will occur.
     The program will exit and report an error if select returns a value of less than 1.  */
    
    //--------------------------------------------------------------------------
    // Main Server Loop
    //--------------------------------------------------------------------------
    
    while(1) {
        char ch = 'A';
        int fd;
        int nread;
        
        testfds = readfds;
        
//        printf("server waiting\n");
        result = select(FD_SETSIZE, &testfds, (fd_set *)0,
                        (fd_set *)0, (struct timeval *) 0);
        
        if(result < 1) {
            perror("server5");
            exit(1);
        }
        
        /*  Once we know we've got activity,
         we find which descriptor it's on by checking each in turn using FD_ISSET.  */
        
        for(fd = 0; fd < FD_SETSIZE; fd++) {
            if(FD_ISSET(fd,&testfds)) {
                
                /*  If the activity is on server_sockfd, it must be a request for a new connection
                 and we add the associated client_sockfd to the descriptor set.  */
                
                if(fd == server_sockfd) {
                    client_len = sizeof(client_address);
                    client_sockfd = accept(server_sockfd,
                                           (struct sockaddr *)&client_address, (socklen_t *)&client_len); // c++ version
                    FD_SET(client_sockfd, &readfds);
                    printf("adding client on fd %d\n", client_sockfd);
                }
                
                /*  If it isn't the server, it must be client activity.
                 If close is received, the client has gone away and we remove it from the descriptor set.
                 Otherwise, we 'serve' the client as in the previous examples.  */
                
                else {
                    ioctl(fd, FIONREAD, &nread);
                    
                    if(nread == 0) {
                        close(fd);
                        FD_CLR(fd, &readfds);
                        printf("removing client on fd %d\n", fd);
                    }
                    else {
                        //------------------------------------------------------
                        // Receive Message
                        //------------------------------------------------------
                        char msgFromClient[256];
                        read(fd, msgFromClient, 256);
//                        sleep(1);
//                        printf("charFromClient is %s\n", msgFromClient);

                        //------------------------------------------------------
                        // Create new thread to proceess request
                        //------------------------------------------------------
                        std::thread{handleRequest, fd, msgFromClient}.detach();
                    }
                }
            }
        }
    }
}
