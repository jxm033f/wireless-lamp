import json
import serial
import socket
import time

LOCAL_UDP_IP = "192.168.1.2"
SHARED_UDP_PORT = 4210

arduino = serial.Serial('/dev/ttyUSB0', 115200)
time.sleep(2)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((LOCAL_UDP_IP, SHARED_UDP_PORT))

with open('current-5day-forecast.json') as f:
    weather_data = json.load(f)

day_num = weather_data['daily']
weather_range = [40, 50, 60]

mode_time = False
mode_api = False

while True:
    data, addr = sock.recvfrom(2048)
    data = data.decode()
    if data and not mode_time and not mode_api:
        if data.isdigit():
            mode_api = True
        elif data == "Time":
            mode_time = True
            
    if data.isdigit() and mode_api:
        num = int(data)
        weather_info = day_num[num-1]['temp']['day']
        print("Weather for Day " + str(num) + " is " + str(weather_info))
        if weather_info <= weather_range[0]:
            #less than equal to 40
            #sock.sendto("BLUE".encode(), ("192.168.1.1", 4210))
            #arduino.write("BLUE")
            arduino.write("B".encode())
            mode_time = False
        elif weather_info > weather_range[0] and weather_info <= weather_range[1]:
            #between 41 and 50
            #sock.sendto("GREEN".encode(), ("192.168.1.1", 4210))
            arduino.write("G".encode())
            mode_time = False
        elif weather_info > weather_range[1] and weather_info <= weather_range[2]:
            #between 51 and 60
            #sock.sendto("YELLOW".encode(), ("192.168.1.1", 4210))
            arduino.write("Y".encode())
            mode_time = False
        elif weather_info > weather_range[2]:
            #greater than 60
            #sock.sendto("RED".encode(), ("192.168.1.1", 4210))
            arduino.write("R".encode())
            mode_time = False