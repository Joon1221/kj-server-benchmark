/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include <string.h>
#include <fstream>

#include <curl/curl.h>

using namespace std;

#define SERVER_CPLUSPLUS 1
#define SERVER_JAVASERVLET 2
#define SERVER_NODEJS 3

int main() {
    int serverType = SERVER_NODEJS;
    
    if (serverType == SERVER_CPLUSPLUS) {
        int sockfd;
        int len;
        struct sockaddr_in address;
        int result;
        char ch = 'A';
        
        /*  Create a socket for the client.  */
        
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        /*  Name the socket, as agreed with the server.  */
        
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr("127.0.0.1");
        address.sin_port = htons(8888);
        len = sizeof(address);
        
        /*  Now connect our socket to the server's socket.  */
        
        result = connect(sockfd, (struct sockaddr *)&address, len);
        
        if(result == -1) {
            perror("oops: client3");
            exit(1);
        }
        
        /*  We can now read/write via sockfd.  */
        while (true) {
    //        read(1, &ch, 1); // same as std::in

            //        write(2, &ch, 1); // same as std::out
            std::cin >> ch;
            write(sockfd, &ch, 1);
            read(sockfd, &ch, 1);
            printf("char from server = %c\n", ch);

        }
        printf("char from server = %c\n", ch);
        close(sockfd);
    }
    else if (serverType == SERVER_JAVASERVLET) {
        CURL *curl;
        CURLcode res;

        /* In windows, this will init the winsock stuff */
        curl_global_init(CURL_GLOBAL_ALL);

        /* get a curl handle */
        curl = curl_easy_init();
        if(curl) {
            /* First set the URL that is about to receive our POST. This URL can
             just as well be a https:// URL if that is what should receive the
             data. */
            curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8080/server");
            /* Now specify the POST data */
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=danie l&project=curl");

            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));

            /* always cleanup */
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
    }
    else if (serverType == SERVER_NODEJS) {
        int sockfd;
        int len;
        struct sockaddr_in address;
        int result;
        char ch = 'A';
        
        /*  Create a socket for the client.  */
        
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        /*  Name the socket, as agreed with the server.  */
        
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr("127.0.0.1");
        address.sin_port = htons(8888);
        len = sizeof(address);
        
        /*  Now connect our socket to the server's socket.  */
        
        result = connect(sockfd, (struct sockaddr *)&address, len);
        
        if(result == -1) {
            perror("oops: client3");
            exit(1);
        }
        char test[] = "B";
        /*  We can now read/write via sockfd.  */
        while (true) {
            //        read(1, &ch, 1); // same as std::in
            
            //        write(2, &ch, 1); // same as std::out

            std::cin >> test;
            write(sockfd, test, 5);

            char textFromServer[256];
            read(sockfd, textFromServer, 100);
            printf("text from server = %s\n", textFromServer);
        }

        printf("char from server = %c\n", ch);
        close(sockfd);
    }
   
    return 0;
}
