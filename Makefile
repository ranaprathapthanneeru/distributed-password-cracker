all:
	g++  server.cpp -o server.out
	g++  client.cpp -o client.out
	g++  worker.cpp -o worker.out -lcrypt
clean:
	rm -f server.out client.out worker.out