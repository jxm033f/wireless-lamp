import json
import socket

LOCAL_UDP_IP = "192.168.1.2"
SHARED_UDP_PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((LOCAL_UDP_IP, SHARED_UDP_PORT))

with open('current-5day-forecast.json') as f:
    weather_data = json.load(f)

daily_weather = weather_data['daily']
print(len(daily_weather))

while True:
    data, addr = sock.recvfrom(2048)
    print(data)
