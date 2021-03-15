# Module 3: Wireless Lamp
COMS BC 3930

Through this module, I created a wireless lamp that reacts to a photoresister and some interactions via buttons and potentiometer. The feautures that this lamp includes are changes color based on current temperature and provides a timer when the lamp will turn automatically off which then the Raspberry Pi will produce a sound through Sonic Pi.

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
Blue Button  - Pin 5 <br />
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
7. Then run python-connection.py on Terminal in wireless-lamp directory
```bash
python3 python-connection.py
```
