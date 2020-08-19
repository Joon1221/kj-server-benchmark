sync: sync.cpp
	g++ -std=c++11 -pthread sync.cpp -I"../../api/openssl-1.1.1c/include" -L"../../api/openssl-1.1.1c" -lssl -lcrypto -o sync 
	./sync
async: async.cpp
	g++ -std=c++11 -pthread async.cpp -I"../../api/openssl-1.1.1c/include" -L"../../api/openssl-1.1.1c" -lssl -lcrypto -o async
	./async 
clean:
	rm -rf sync
	rm -rf async