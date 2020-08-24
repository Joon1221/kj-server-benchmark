# KJ Server Benchmark


kj_server_benchmark is a tool used to compare the performance of different 
server set-ups so users can make a more informed decisions on what server 
archetype to use for different scenarios. 

<p align="center">
  <img src="./assets/demo.png" alt="Size Limit CLI" width="600">
</p>

Features
--------

- Runs tests on 4 server types (**C++ Asynchronous**, **C++ Synchronous**, **JavaServlet** using **Apache Tomcat**, and **Node.js**).
- Tracks **ping** and **bitrate**.
- Runs tests on 2 scenarios (**Chat** server and **Streaming** server).

Installation
------------

### Step 1: Install and Set Up Ubuntu Server as a Virtual Machine
Download VirtualBox (6.0.8): https://www.virtualbox.org/wiki/Downloads
	
<br>	

Download Ubuntu Server (18.04.2 LTS): https://ubuntu.com/download/server

<br>	

Turn on Virtual Box and create new virtual machine (*Note that specifications do not need to be identical*)

```
Ram: 4096 MB
Storage: 10.0 GB
Type: Linux
Version: Ubuntu (64-bit)
```

Set bootable device to the downloaded Ubuntu iso file.
- Enter `Settings -> Storage`
- Click on the empty disk under *`Controller: IDE`.
- Next to `Optical Drive:`, click on the disk icon, and select
	  the downloaded iso file.

<br>		  
	  
Start the device and follow the instructions to initialize the system.

<br>	

Enter `Settings -> Storage` once again and eject the startup disk.

### Step 2: Install C++ Prerequisites

<br>

*Move all downloaded files to `kj_server_benchmark/api`.*

<br>

Download cmake to install libraries.

- MacOS:
	``` Shell
	/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
	
	
	brew install cmake
	```
- Linux: 
	``` Shell
	sudo apt-get install linuxbrew-wrapper


	sudo apt install cmake
	```	 

Download and extract jsoncpp (1.8.0): https://github.com/open-source-parsers/jsoncpp

``` Shell
cd kj_server_benchmark/api/jsoncpp-master


mkdir -p build/debug 


cd build/debug


cmake -DCMAKE_BUILD_TYPE=debug -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DARCHIVE_INSTALL_DIR=. -G "Unix Makefiles" ../..


make
```

Download and extract openssl (1.1.1c): https://www.openssl.org/source/

- MacOS:
	``` Shell

	cd kj_server_benchmark/api/openssl-1.1.1c
	
	
	./Configure darwin64-x86_64-cc --openssldir=/usr/local/ssl
	
	
	make
	
	
	make test
	```

- Linux:
	``` Shell
	./config -–prefix=.../kj_server_benchmark/api/openssl-1.1.1c -–openssldir=.../kj_server_benchmark/api/openssl-1.1.1c
	
	
	make -j8
	
	
	sudo make install
	```
	*Make sure to replace ... with the location of the project.*


Download and extract curl (7.65.1): https://curl.haxx.se/download.html

``` Shell
./configure --prefix=/usr/local/curl


make


sudo make install
```
### Step 2: Install C++ Prerequisites

<br>

*Move all downloaded files to `kj_server_benchmark/api`.*

<br>

Download java. 
``` Shell
apt-get install default-jdk


java -version 


nano ~/.bashrc
```

Paste the following at the very bottom (*replace X with the java version found previously*)

``` Shell
export JAVA_HOME=/usr/lib/jvm/java-X-openjdk-amd64
```

Download and extract apache tomcat (9.0.21): https://tomcat.apache.org/download-90.cgi

``` Shell
cd kj_server_benchmark/api/tomcat/apache-tomcat-9.0.21/conf


keytool  -genkey -alias server-alias -keyalg RSA -keypass changeit -storepass changeit -keystore localhost.jks


nano server.xml
```

Paste the following underneath `<Connector port="8080" ... redirectPort="8443" />`

``` XML
- <Connector
	protocol="HTTP/1.1"
	port="8443" maxThreads="200"
	scheme="https" secure="true" SSLEnabled="true"
	keystoreFile="conf\localhost.jks" keystorePass="xxxx"
	clinetAuth="false" sslProtocol="TLS" />
```

Create folders for libraries and classes
``` Shell
cd kj_server_benchmark/api/tomcat/apache-tomcat-9.0.21/webapps/ROOT/WEB-INF


mkdir classes


mkdir lib
```

Download gson (2.8.5): https://search.maven.org/artifact/com.google.code.gson/gson/2.8.5/jar

<br>

Move the .jar file to kj_server_benchmark/api/gson

<br>

Create a copy and move the copy to kj_server_benchmark/api/tomcat/apache-tomcat-9.0.21/webapps/ROOT/WEB-INF/classes

<br>

Download web.xml from git repository and replace it with the web.xml located at kj_server_benchmark/api/tomcat/apache-tomcat-9.0.21/webapps/ROOT/WEB-INF
	
#### Node.js:
``` Shell
cd kj_server_benchmark/api
npm install express@">=3.0.0 <4.0.0" --save
```

### Step 3: Update Header and Library Search Paths (Optional)
*Note* If all the steps above are done exactly as stated, with all api 
downloaded to the same version, this step can be skipped. The steps will be 
separated for each library.

- Openssl
	- Locate and open 2019_summer_project_client.xcodeproj. It should be 
	  located at kj_server_benchmark/client/client
	- Click on the blue icon located near the top left
	- Go to Build Settings and locate the two flags "Header Search Paths"
	  and "Library Search Paths"
	- Update "Header Search Paths" to the correct openssl version
		- Change ../../api/openssl-1.1.1c/include to ../../api/openssl-X.X.Xx/include 
	- Update "Library Search Paths" to the correct openssl version
		- Change ../../api/openssl-1.1.1c to ../../api/openssl-X.X.Xx
	- Locate and open the makefile for the c++ servers. It should be located
	  at kj_server_benchmark/server/c++
	- Update the header search flag (both instances)
		- Change -I"../../api/openssl-1.1.1c/include" to -I"../../api/openssl-X.X.Xx/include"
	- Update the library search flag (both instances)
		- Change -L"../../openssl-1.1.1c" to -L"../../openssl-X.X.Xx"
- Tomcat
	- Locate and open the makefile for the java servlet server. It should 
	  be located at kj_server_benchmark/server/javaservlet
	- Update all five instances of apache-tomcat-9.0.21
		- Change apache-tomcat-9.0.21 to apache-tomcat-X.X.XX

- gson
	- Locate and open the makefile for the java servlet server. It should 
	  be located at kj_server_benchmark/server/javaservlet
	- Update class path
		- Change ../../api/gson/gson-2.8.5.jar to ../../api/gson/gson-X.X.X.jar



Usage
-----

Instructions on how to run the program can be found on USERS_GUIDE.txt 


Libraries
---------

- curl 7.65.1
- gson 2.8.5
- jsoncpp 1.8.0
- openssl 1.1.1c
- node modules 


Goals
-----

-Test the performance between the C++ Synchronous, C++ Asynchronous, JavaServlet,
  and Node.js servers
  
  
-Test the performance between Oracle Database and MySQL Database


-Create multiple tests to simulate different environment
	- Gaming
		- Real Time
		- Turn-based
	- Chatting
	- Downloading
	- Simple Websites
	- Video/Music Streaming
	- Transactions
Visually represent the server performance 


Design Decisions
----------------

- Encrypt messages using RSA algorithm and HTTPS to more closely resemble reality
- The original plan was to test the C++ server in three different environments: 
  Non-Binding Socket, Linux EPOLL, and Windows IOCP
	- We will not be using Windows IOCP 
- Create two separate Ubuntu virtual machines
	- One is for the server itself and one is for the database
	

	
Staff
-----

Programmer : Joon Kang


Mentor : Samil Chai


Work Period
-----------

Start Date: 2019-07-02 


End Date: 2019-09-06







