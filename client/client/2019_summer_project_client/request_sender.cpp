//
//  request_sender.cpp
//  2019_summer_project_client
//
//  Created by Joon Kang on 2019-08-02.
//  Copyright © 2019 Joon Kang. All rights reserved.
//

#include "request_sender.h"

#include <iostream>

using namespace std;

RequestSender::RequestSender() {
    ip_address = "127.0.0.1";
    port = "1111";
    extension = "";
    startSendingRequest = false;
    numFulfilledRequests = 0;
//    cout << "ip_address: " << ip_address << endl;
}
RequestSender::RequestSender(string ip_address, string port, string extension) {
    this->ip_address = ip_address;
    this->port = port;
    this->extension = extension;
    startSendingRequest = false;
    numFulfilledRequests = 0;
//    cout << "ip_address: " << ip_address << endl;
}
