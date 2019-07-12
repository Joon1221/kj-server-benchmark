all: run
run: server
	sh /Users/joonkang/Documents/2019_summer_project/tomcat/apache-tomcat-9.0.21/bin/startup.sh
server: server
	javac -cp ../../tomcat/apache-tomcat-9.0.21/lib/servlet-api.jar server.java
stop:
	sh /Users/joonkang/Documents/2019_summer_project/tomcat/apache-tomcat-9.0.21/bin/shutdown.sh
clean:
	rm -rf server.class