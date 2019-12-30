import socket

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 65433       # Port to listen on (non-privileged ports are > 1023)
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket_type = socket.getaddrinfo(HOST,PORT)[0][0]
print(socket_type)
if socket_type == socket.AF_INET6 :
	s = socket.socket(socket.AF_INET6, socket.SOCK_STREAM)
else :
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen()
conn, addr = s.accept()
with conn:
	print('Connected by', addr)
	while True:
		data = conn.recv(1024)
		print('Message Received', repr(data))
		if not data:
			break
		a=input("Message Sent: ")
		conn.sendall(a.encode())
