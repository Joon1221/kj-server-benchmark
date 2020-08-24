//
//  request_sender.hpp
//  2019_summer_project_client
//
//  Created by Joon Kang on 2019-08-02.
//  Copyright © 2019 Joon Kang. All rights reserved.
//

#ifndef REQUEST_SENDER_HPP
#define REQUEST_SENDER_HPP

#define VIDEO_REQUEST 0
#define CHAT_REQUEST 1

#include <stdio.h>
#include <string>

using namespace std;

class RequestSender {
public:
    string ip_address;
    string port;
    string extension;
    bool startSendingRequest;
    int numFulfilledRequests;

    RequestSender();
    RequestSender(string ip_address, string port, string extension);
    
    virtual bool sendMultipleRequests(int numSimultaneousRequests, int id) = 0;
    
private:
    virtual bool sendRequest(int id) = 0;
};

#endif 
