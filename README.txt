================================================================================
                               kj_server_benchmark                                                       
================================================================================


INTRODUCTION
------------

kj_server_benchmark is a tool used to compare the performance of different 
server set-ups. It tracks information such as bitrate and ping, allowing users
to make a more informed decision on what server archetype to use for each 
scenario. It currently runs tests on 4 different server types (C++ Asynchronous,
C++ Synchronous, JavaServlet using Apache Tomcat, and Node.js) and 2 different 
scenarios (Chat server and Streaming server).


STAFF
-----

Lead Programmer : Joon Kang
Technical Director : Samil Chai


WORK PERIOD
-----------

Start Date: 2019-07-02 
End Date: 2019-09-06


INSTALLATION
------------

Installation guide can be found on USERS_GUIDE.txt


HOW TO USE
----------

Instructions on how to run the program can be found on USERS_GUIDE.txt 


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


DESIGN DECISIONS
----------------
- Encrypt messages using RSA algorithm to more closely resemble reality
- The original plan was to test the C++ server in three different environments: 
  Non-Binding Socket, Linux EPOLL, and Windows IOCP
	- We will not be using Windows IOCP 
- Create two separate Ubuntu virtual machines
	- One is for the server itself and one is for the database

