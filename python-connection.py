from psonic import *
import json
import random
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
mode_alarm = True

hms_option = 0;
time_holder = 0;
hour_time = 0;
min_time = 0;
sec_time = 0;
selected_time = ""

while True:
    data, addr = sock.recvfrom(2048)
    data = data.decode()
    if data and not mode_time and not mode_api:
        if data == "API":
            print("API IS TURNED ON")
            mode_api = True
        elif data == "TIME":
            print("TIME IS BEING CONFIGURED")
            hms_option = 0
            mode_time = True
            
    if data.isdigit() and mode_api:
        num = int(data)
        weather_info = day_num[num-1]['temp']['day']
        print("Weather for Day " + str(num) + " is " + str(weather_info))
        if weather_info <= weather_range[0]:
            arduino.write("B".encode())
        elif weather_info > weather_range[0] and weather_info <= weather_range[1]:
            arduino.write("G".encode())
        elif weather_info > weather_range[1] and weather_info <= weather_range[2]:
            arduino.write("Y".encode())
        elif weather_info > weather_range[2]:
            arduino.write("R".encode())
    elif data == "CANCEL" and mode_api:
        print("API IS TURNED OFF")
        mode_api = False
            
    if data and mode_time:
        if data == "MIN":
            hour_time = time_holder
            hms_option = hms_option + 1
        elif data == "SEC":
            min_time = time_holder
            hms_option = hms_option + 1
        elif data == "CONFIRM":
            sec_time = time_holder
            hms_option = hms_option + 1
        if data.isdigit():
            if hms_option == 0:
                print("Current Choosen Hour: " + data)
                time_holder = int(data)
            elif hms_option == 1:
                print("Current Choosen Minute: " + data)
                time_holder = int(data)
            elif hms_option == 2:
                print("Current Choosen Second: " + data)
                time_holder = int(data)
        if hms_option == 3:
            if hour_time < 10:
                str_hour = "0" + str(hour_time)
            else:
                str_hour = str(hour_time)
            if min_time < 10:
                str_min = "0" + str(min_time)
            else:
                str_min = str(min_time)
            if sec_time < 10:
                str_sec = "0" + str(sec_time)
            else:
                str_sec = str(sec_time)
            selected_time = str_hour + ":" + str_min + ":" + str_sec
            print("Time selected is " + selected_time)
            mode_time = False
            mode_alarm = True
    
    if data and mode_alarm:
        if selected_time == data:
            print("HOORAY")
            mode_alarm = False
            arduino.write("F".encode())
            for i in range(5):
                play(random.randrange(50, 100))
                sleep(0.5)