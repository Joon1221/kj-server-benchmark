//
//  request_sender_binary.cpp
//  2019_summer_project_client
//
//  Created by Joon Kang on 2019-08-02.
//  Copyright © 2019 Joon Kang. All rights reserved.
//

#include "request_sender_binary.h"

// c-socket
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

// openssl
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>

// etc
#include <iostream>
#include <thread>
#include "timer.h"


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

RequestSenderBinary::RequestSenderBinary() : RequestSender() {
    rsaEncrypt = createRSAWithFilename("../../public.pem", true);
    rsaDecrypt = createRSAWithFilename("../../private.pem", false);
}
RequestSenderBinary::RequestSenderBinary(string ip_address, string port, string extension) : RequestSender(ip_address, port, extension) {
    rsaEncrypt = createRSAWithFilename("../../public.pem", true);
    rsaDecrypt = createRSAWithFilename("../../private.pem", false);
}

bool RequestSenderBinary::sendRequest(int id) {
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    
    /*  Create a socket for the client.  */
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    /*  Name the socket, as agreed with the server.  */
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip_address.c_str());
    address.sin_port = htons(std::atoi(port.c_str()));
    len = sizeof(address);
    
    /*  Now connect our socket to the server's socket.  */
    
    result = connect(sockfd, (struct sockaddr *)&address, len);
    
    if(result == -1) {
        perror("oops: client");
        sendRequest(id);
        exit(1);
    }
    //------------------------------------------------------------------
    // Main Client Loop
    //------------------------------------------------------------------
    
    while (!startSendingRequest) { } // Used to stall threads so they start simultaneously
    //------------------------------------------------------------------
    // Encrypt Message
    //------------------------------------------------------------------
    int msg = id;  // Message to encrypt
    char msgEncrypted[256];    // Encrypted message
    char *err;               // Buffer for any error messages

    // Encrypt the message
    err = (char *)malloc(130);
    if((RSA_public_encrypt(8, (unsigned char*)&msg, (unsigned char*)msgEncrypted,
                           rsaEncrypt, RSA_PKCS1_OAEP_PADDING)) == -1) {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error encrypting message: %s\n", err);
    }

    //------------------------------------------------------------------
    // Send Message
    //------------------------------------------------------------------
    write(sockfd, msgEncrypted, 256);

    //------------------------------------------------------------------
    // Receive Message
    //------------------------------------------------------------------
    if (id == VIDEO_REQUEST) {
        for (int i = 0; i < 164017; i++) {
            char textFromServer[256];
            int numBytesRead = read(sockfd, textFromServer, 256);
        }
    }
    else if (id == CHAT_REQUEST) {
        char textFromServer[256];
        int numBytesRead = read(sockfd, textFromServer, 256);
//        char msgDecrypted[256];    // Decrypted message
//
//        // Decrypt it
//        if(RSA_private_decrypt(256, (unsigned char*)textFromServer, (unsigned char*)msgDecrypted,
//                               rsaDecrypt, RSA_PKCS1_OAEP_PADDING) == -1) {
//            ERR_load_crypto_strings();
//            ERR_error_string(ERR_get_error(), err);
//            fprintf(stderr, "Error decrypting message: %s\n", err);
//        }
//        else {
//            //        numFulfilledRequests++;
//        }
//        printf("Decrypted message: %s\n id: %d\n", msgDecrypted, id);
    }
    
    //------------------------------------------------------------------
    // Decrypt Message
    //------------------------------------------------------------------
    numFulfilledRequests++;

    close(sockfd);

    return true;
}

bool RequestSenderBinary::sendMultipleRequests(int numSimultaneousRequests, int id) {
    numFulfilledRequests = 0;
    for (int i = 0; i < numSimultaneousRequests; i++) {
        std::thread {&RequestSenderBinary::sendRequest, this, id}.detach();
    }

    usleep(2000000);
    startSendingRequest = true;
   
    Timer *timer = new Timer();
    timer->startTimer();
    
    while (numFulfilledRequests < numSimultaneousRequests) {
    }
    timer->stopTimer();
    timer->displayTime();
    
    if (id == VIDEO_REQUEST) {
        cout << "Bitrate: " << 164017*256*8/timer->getTimeTaken() << " bps" << endl;
    }
    else if (id == CHAT_REQUEST) {
        cout << "Bitrate: " << 256*8/timer->getTimeTaken() << " bps" << endl;
    }
//    cout << "FINISHED" << endl;
    
    return true;
}

