README

For Server:
g++ server.cpp -o s -lpthread 
./s <Server Port>

For Client 1:
g++ client.cpp -o c -lpthread
./c <Server Address> <Server Port> <Message>

For Client 2:
g++ client.cpp -o c -lpthread
./c <Server Address> <Server Port> <Message>