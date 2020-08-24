//
//  request_sender_http.cpp
//  2019_summer_project_client
//
//  Created by Joon Kang on 2019-08-02.
//  Copyright © 2019 Joon Kang. All rights reserved.
//

#include "request_sender_http.h"

// curl
#include <curl/curl.h>

// jsoncpp

#include <json/json.h>

// etc
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <unistd.h>
#include "timer.h"

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

RequestSenderHTTP::RequestSenderHTTP() : RequestSender() {
    
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    //    ip_address = "127.0.0.1";
    //    cout << "ip_address: " << ip_address << endl;
}
RequestSenderHTTP::RequestSenderHTTP(string ip_address, string port, string extension) : RequestSender(ip_address, port, extension) {
    
    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);
}

// thread function
bool RequestSenderHTTP::sendRequest(int id) {
    while (!startSendingRequest) {} // Used to stall threads so they start simultaneously


    CURL *curl;
    CURLcode res;

    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {
        // ---------------------------------------------------------------------
        // create json object
        // ---------------------------------------------------------------------
        Json::Value value(Json::objectValue);
        value["name"] = "TestName";
        stringstream convert;
        convert << id;
        value["id"] = convert.str().c_str();

        Json::StreamWriterBuilder builder;
        builder["indentation"] = ""; // If you want whitespace-less output
        const std::string output = Json::writeString(builder, value);

        char *jsonObj = (char *)output.c_str();

        
        // ---------------------------------------------------------------------
        // create http packet
        // ---------------------------------------------------------------------

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        curl_easy_setopt(curl, CURLOPT_URL, string(string("https://") + ip_address.c_str() + string(":") + port.c_str() + extension.c_str()).c_str());

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL , 1L);

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.65.1");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); // comment out to enable printing
//                curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        
//                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        // ---------------------------------------------------------------------
        // send http packet
        // ---------------------------------------------------------------------

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            sendRequest(id);
        }
        else {
            curl_easy_cleanup(curl);
            numFulfilledRequests++;
        }
    }

    return true;
}

bool RequestSenderHTTP::sendMultipleRequests(int numSimultaneousRequests, int id) {
    numFulfilledRequests = 0;

    for (int i = 0; i < numSimultaneousRequests; i++) {
        // create new thread
        std::thread {&RequestSenderHTTP::sendRequest, this, id}.detach();
    }

    usleep(2000000); // gives time for all threads to be ready
    startSendingRequest = true; // signal to start all thread simultaneously

    Timer *timer = new Timer();
    timer->startTimer();
    
    while (numFulfilledRequests < numSimultaneousRequests) { }
    
    timer->stopTimer();
    timer->displayTime();
    
    // display results
    if (id == VIDEO_REQUEST) {
        cout << "Bitrate: Not implemented yet" << endl;
    }
    else if (id == CHAT_REQUEST) {
        cout << "Bitrate: Not implemented yet" << endl;
    }
//    cout << "FINISHED" << endl;
    curl_global_cleanup();

    return true;
}

