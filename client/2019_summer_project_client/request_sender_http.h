//
//  request_sender_http.hpp
//  2019_summer_project_client
//
//  Created by Joon Kang on 2019-08-02.
//  Copyright © 2019 Joon Kang. All rights reserved.
//

#ifndef REQUEST_SENDER_HTTP_HPP
#define REQUEST_SENDER_HTTP_HPP

#include <stdio.h>

#include "request_sender.h"

class RequestSenderHTTP : public RequestSender {
private:

public:
    RequestSenderHTTP();
    RequestSenderHTTP(string ip_address, string port, string extension);

    bool sendMultipleRequests(int numSimultaneousRequests, int id);

private:
    bool sendRequest(int id);
    
};

#endif /* request_sender_http_hpp */
