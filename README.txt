================================================================================
                               kj_server_benchmark                                                       
================================================================================


INTRODUCTION
------------

kj_server_benchmark is a tool used to compare the performance of different 
server set-ups. It tracks information such as bitrate and ping, which allows
users to make a more informed decision on what server archetype to use for  
different scenarios. It currently runs tests on 4 different server types (C++ Asynchronous, C++ Synchronous, JavaServlet using Apache Tomcat, and Node.js) 
and 2 different scenarios (Chat server and Streaming server).


STAFF
-----

Programmer : Joon Kang
Mentor : Samil Chai


WORK PERIOD
-----------

Start Date: 2019-07-02 
End Date: 2019-09-06


GOALS
-----

- Test the performance between the C++ Synchronous, C++ Asynchronous, JavaServlet,
  and Node.js servers
- Test the performance between Oracle Database and MySQL Database
- Create multiple tests to simulate different environment
	- Gaming
		- Real Time
		- Turn-based
	- Chatting
	- Downloading
	- Simple Websites
	- Video/Music Streaming
	- Transactions
- Visually represent the server performance 


INSTALLATION
------------

Installation guide can be found on USERS_GUIDE.txt


HOW TO USE
----------

Instructions on how to run the program can be found on USERS_GUIDE.txt 


LIBRARIES
---------

- curl 7.65.1
- gson 2.8.5
- jsoncpp 1.8.0
- openssl 1.1.1c
- node modules 


MULTIPLE CONNECTIONS
--------------------

Since servers handle workload from multiple connections at once, threads are used
on the client side to simulate multiple connections happening simultaneously. 
The number of threads can be changed to simulate different levels of server stress,
which can be helpful to determine which server archetypes are better at handling
stress than others.


DESIGN DECISIONS
----------------

- Encrypt messages using RSA algorithm and HTTPS to more closely resemble reality
- The original plan was to test the C++ server in three different environments: 
  Non-Binding Socket, Linux EPOLL, and Windows IOCP
	- We will not be using Windows IOCP 
- Create two separate Ubuntu virtual machines
	- One is for the server itself and one is for the database



