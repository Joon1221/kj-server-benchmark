/*  Make the necessary includes and set up the variables.  */


#include <iostream>

#include <string.h>
#include <fstream>
#include <unistd.h>

#include "request_sender.h"
#include "request_sender_binary.h"
#include "request_sender_http.h"

#include "timer.h"

#include <thread>
using namespace std;

#define SERVER_CPLUSPLUS 1
#define SERVER_JAVASERVLET 2
#define SERVER_NODEJS 3

#define KEY_LENGTH 2048

//#define IP_ADDRESS "127.0.0.1"
#define IP_ADDRESS "192.168.1.76"

int main() {
//    int serverType = SERVER_CPLUSPLUS;
////    int serverType = SERVER_JAVASERVLET;
////    int serverType = SERVER_NODEJS;
//
//    if (serverType == SERVER_CPLUSPLUS) {
////        RequestSenderBinary *requestSender = new RequestSenderBinary("127.0.0.1");
//        RequestSenderBinary *requestSender = new RequestSenderBinary("192.168.1.76");
//        requestSender->sendMultipleRequests(500, CHAT_REQUEST);
//    }
//    else if (serverType == SERVER_JAVASERVLET) {
////        RequestSenderHTTP *requestSender = new RequestSenderHTTP("http://127.0.0.1:8080/server");
//        RequestSenderHTTP *requestSender = new RequestSenderHTTP("https://127.0.0.1:8443/server");
////        RequestSenderHTTP *requestSender = new RequestSenderHTTP("https://192.168.1.73:8443/server");
//        requestSender->sendMultipleRequests(500, VIDEO_REQUEST);
//    }
//    else if (serverType == SERVER_NODEJS) {
//        RequestSenderHTTP *requestSender = new RequestSenderHTTP("https://127.0.0.1:8443");
////        RequestSenderHTTP *requestSender = new RequestSenderHTTP("https://192.168.1.73:8443");
//        requestSender->sendMultipleRequests(500, VIDEO_REQUEST);
//    }

//        RequestSenderBinary *requestSender = new RequestSenderBinary("127.0.0.1");
    
    int numChatRequests = 0;
    int numVideoRequests = 0;
    
    cout << "Number of chat requests sent simultaneously: ";
    cin >> numChatRequests;
    cout << "Number of video requests sent simultaneously: ";
    cin >> numVideoRequests;
    
    //==========================================================================
    // C++ Synchronous
    //==========================================================================
    cout << "================================================================================" << endl;
    cout << "C++ Socket Server Test" << endl;
    cout << "================================================================================" << endl;
    RequestSenderBinary *requestSenderSync = new RequestSenderBinary(IP_ADDRESS, "8888", "");
    
    cout << "Chat Request Test (Encrypted)" << endl;
    requestSenderSync->sendMultipleRequests(500, CHAT_REQUEST);
    cout << "Video Request Test (Encrypted)" << endl;
    requestSenderSync->sendMultipleRequests(5, VIDEO_REQUEST);

    //==========================================================================
    // C++ Asynchronous
    //==========================================================================
    cout << "================================================================================" << endl;
    cout << "C++ Epoll Server Test" << endl;
    cout << "================================================================================" << endl;
    RequestSenderBinary *requestSenderAsync = new RequestSenderBinary(IP_ADDRESS, "7777", "");

    cout << "Chat Request Test (Encrypted)" << endl;
    requestSenderAsync->sendMultipleRequests(numChatRequests, CHAT_REQUEST);
    cout << "Video Request Test (Encrypted)" << endl;
    requestSenderAsync->sendMultipleRequests(numVideoRequests, VIDEO_REQUEST);
    
    //==========================================================================
    // Java Servlet
    //==========================================================================
    cout << "================================================================================" << endl;
    cout << "Java Servlet Server Test" << endl;
    cout << "================================================================================" << endl;
    RequestSenderHTTP *requestSenderJava = new RequestSenderHTTP(IP_ADDRESS, "8443", "/server");

    cout << "Chat Request Test" << endl;
    requestSenderJava->sendMultipleRequests(numChatRequests, CHAT_REQUEST);
    cout << "Video Request Test" << endl;
    requestSenderJava->sendMultipleRequests(numVideoRequests, VIDEO_REQUEST);

    //==========================================================================
    // Node.js
    //==========================================================================
    cout << "================================================================================" << endl;
    cout << "Node.js Server Test" << endl;
    cout << "================================================================================" << endl;
    RequestSenderHTTP *requestSenderNode = new RequestSenderHTTP(IP_ADDRESS, "7443", "");
    
    cout << "Chat Request Test" << endl;
    requestSenderNode->sendMultipleRequests(numChatRequests, CHAT_REQUEST);
//    cout << "Video Request Test" << endl;
//    requestSenderNode->sendMultipleRequests(numVideoRequests, VIDEO_REQUEST);
    
    return 0;
}
