readme
List of files:
worker.cpp
client.cpp
server.cpp
Makefile

Running instructions:
Run "make" to compile. It creates executables.
run
./server.out <port-number>
./worker.out <ip-address> <port-number>
./client.out <ip-address> <port-number> <hashed-password> <password-length> <3-bit>
in different terminals(systems)

Note: 
Before running make, change the ip-address in server.cpp to the ip address of the system on which server has to be started. 

