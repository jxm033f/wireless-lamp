# Module 3: Wireless Lamp
COMS BC 3930

Through this module, I created a wireless lamp that reacts to a photoresister and some interactions via buttons and potentiometer. The feautures that this lamp includes are brightness based on light in the room. Another feature it provides is a timer which will allow the lamp to turn off automatically but then the Raspberry Pi will also produce a sound through Sonic Pi.

Another feature that I was really excited about was using the Weather API or weather data to change the lamp color based on the chosen day's temperature. I was only able to implement it by having the ESP32 being wired to the Raspberry Pi. If interested in seeing how it is implemented check out: https://github.com/jxm033f/wired-lamp

## Materials
- Raspberry Pi
- ESP32
- 2 large buttons
- 1 Potentiometer
- 1 Photoresistor
- 1 LiPo Battery
- 8 LEDs (2 Green, 2 Blue, 2 Yellow, 2 Red)
- 1 10k Ohm Resistor
- 22 Jumper Wires

## Hardware Configuration
### Buttons
Blue Button - Pin 5 <br />
The blue button only works for the wired lamp and not the wireless lamp <br />
Green Button - Pin 4 <br />

### Potentiometer
Pin 35

### Photoresistor
Pin 34

### LEDs
Green 1 - Pin 14 <br />
Green 2 - Pin 13 <br />
Blue 1 - Pin 12 <br />
Blue 2 - Pin 27 <br />
Yellow 1 - Pin 19 <br />
Yellow 2 - Pin 0 <br />
Red 1 - Pin 18 <br />
Red 2 - Pin 15 <br />

## Executing Program
1. Go to [Arduino Site](https://www.arduino.cc/en/software)
2. Download Linux ARM 32-bit (or one based on correct Operating System)
3. Extract file and install the Arduino IDE in dowloaded directory (eg. Downloads)
```bash
cd Downloads
tar -xvf arduino-1.8.13-linuxarm.tar.xz
cd arduino-1.8.13
sudo ./install.sh
```
4. Clone wireless-lamp
```bash
git clone https://github.com/jxm033f/wireless-lamp.git
cd wireless-lamp
```
5. Open Arduino IDE -> File -> Open -> lamp-connection.ino <br />
        1. File is located wireless-lamp/lamp-connection
	2. Install the Time library by Sketch -> Include Library <br />
6. Upload Code which will allow it to run through the ESP32 <br />
        1. Change wifi connection from your network to ESP32
        2. After succesfully uploading you can disconnect the comp from the ESP32
		1. Plug in LiPo battery or other power supply
		2. Click the RST button on the ESP32 to start program
7. Before running the python script open the Sonic Pi software
	1. If not dowloaded:
		1. Go to [Sonic Pi](https://sonic-pi.net/#rp) and download the DEF File.
		2. Run the following Commands:
```bash
sudo apt update
sudo apt install ./sonic-pi_3.3.1_1_armhf.deb
```
8. Then run python-connection.py on Terminal in wireless-lamp directory
```bash
python3 python-connection.py
```

## Links
Blog: http://edblogs.columbia.edu/comsx3930-001-2021-1/2021/03/15/module-3-timed-lamp/ <br />
Video 1: https://youtu.be/JJ3zeaau3Vo <br />
Video 2: https://youtu.be/3AkQJ_7euQE <br />
