import socket

LOCAL_UDP_IP = "192.168.1.2"
SHARED_UDP_PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((LOCAL_UDP_IP, SHARED_UDP_PORT))

while True:
	data, addr = sock.recvfrom(2048)
	print(data)
