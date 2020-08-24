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
//#define IP_ADDRESS "192.168.1.76"

int main() {
    
    int numChatRequests = 0;
    int numVideoRequests = 0;
    string ipAddressStr;
    
    cout << "IP Address: ";
    cin >> ipAddressStr;
    
    const char *IP_ADDRESS = ipAddressStr.c_str();
    
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
