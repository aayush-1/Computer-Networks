import socket

HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 65433

s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket_type = socket.getaddrinfo(HOST,PORT)[0][0]
print(socket_type)
if socket_type == socket.AF_INET6 :
	s = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
else :
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
while(1):
	a=input("Message Sent: ")
	s.sendall(a.encode())
	data = s.recv(1024)
	print('Message Received', repr(data))

