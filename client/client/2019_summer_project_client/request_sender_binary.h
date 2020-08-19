//
//  request_sender_binary.hpp
//  2019_summer_project_client
//
//  Created by Joon Kang on 2019-08-02.
//  Copyright © 2019 Joon Kang. All rights reserved.
//

#ifndef REQUEST_SENDER_BINARY_HPP
#define REQUEST_SENDER_BINARY_HPP

#include <stdio.h>

#include "request_sender.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bio.h>

class RequestSenderBinary : public RequestSender {
private:
//    int sockfd;
    RSA *rsaEncrypt;
    RSA *rsaDecrypt;


public:
    RequestSenderBinary();
    RequestSenderBinary(string ip_address, string port, string extension);
    
    bool sendMultipleRequests(int numSimultaneousRequests, int id);

private:
    bool sendRequest(int id);
};

#endif /* request_sender_binary_hpp */
