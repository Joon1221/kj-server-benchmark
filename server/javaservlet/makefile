all: run
run: server
	sh ../../api/tomcat/apache-tomcat-9.0.21/bin/startup.sh
server: server.java
	javac -cp ../../api/tomcat/apache-tomcat-9.0.21/lib/servlet-api.jar:../../api/gson/gson-2.8.5.jar server.java
	rm -rf ../../api/tomcat/apache-tomcat-9.0.21/webapps/ROOT/WEB-INF/classes/server.class
	mv server.class ../../api/tomcat/apache-tomcat-9.0.21/webapps/ROOT/WEB-INF/classes
stop:
	sh ../../api/tomcat/apache-tomcat-9.0.21/bin/shutdown.sh
clean:
	rm -rf server.class