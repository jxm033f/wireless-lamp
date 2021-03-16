#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Time.h>

#define GREEN_LED1      14
#define GREEN_LED2      13
#define BLUE_LED1       12
#define BLUE_LED2       27
#define YELLOW_LED1     19
#define YELLOW_LED2     0
#define RED_LED1        18
#define RED_LED2        15
bool lamp_working = true;

#define API_BUTTON      5
bool api_isPressed = false;
bool api_beingSet = false;
bool api_status = false;

#define ALARM_BUTTON    4
bool alarm_isPressed = false;
bool time_beingSet = false;
bool hour_isSet = false;
bool min_isSet = false;
bool sec_isSet = false;
bool alarm_isOn = false;
time_t t;

int current_hour = 0;
int prev_hour = 0;
int current_min = 0;
int prev_min = 0;
int current_sec = 0;
int prev_sec = 0;

int hour_timer;
int min_timer;
int sec_timer;

#define CONSOLE_IP "192.168.1.2"
#define CONSOLE_PORT 4210
const char* ssid = "ESP32";
const char* password = "12345678";
WiFiUDP Udp;
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);
char packetBuffer[255];
String currentLine;

int photoValue = 0;
int potenValue = 0;
int prevPoten = 0;

void clear_leds() {
  digitalWrite(BLUE_LED1, LOW);
  digitalWrite(BLUE_LED2, LOW);
  digitalWrite(GREEN_LED1, LOW);
  digitalWrite(GREEN_LED2, LOW);
  digitalWrite(YELLOW_LED1, LOW);
  digitalWrite(YELLOW_LED2, LOW);
  digitalWrite(RED_LED1, LOW);
  digitalWrite(RED_LED2, LOW);
}

void clear_blue() {
  digitalWrite(BLUE_LED1, LOW);
  digitalWrite(BLUE_LED2, LOW);
}

void clear_green() {
  digitalWrite(GREEN_LED1, LOW);
  digitalWrite(GREEN_LED2, LOW);
}

void clear_yellow() {
  digitalWrite(YELLOW_LED1, LOW);
  digitalWrite(YELLOW_LED2, LOW);
}

void clear_red() {
  digitalWrite(RED_LED1, LOW);
  digitalWrite(RED_LED2, LOW);
}

void fill_color(String color) {
  if (color == "GREEN") {
    digitalWrite(GREEN_LED1, HIGH);
    digitalWrite(GREEN_LED2, HIGH);
  } else if (color == "BLUE") {
    digitalWrite(BLUE_LED1, HIGH);
    digitalWrite(BLUE_LED2, HIGH);
  } else if (color == "YELLOW") {
    digitalWrite(YELLOW_LED1, HIGH);
    digitalWrite(YELLOW_LED2, HIGH);
  } else if (color == "RED") {
    digitalWrite(RED_LED1, HIGH);
    digitalWrite(RED_LED2, HIGH);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(GREEN_LED1, OUTPUT);
  pinMode(GREEN_LED2, OUTPUT);
  pinMode(BLUE_LED1, OUTPUT);
  pinMode(BLUE_LED2, OUTPUT);
  pinMode(YELLOW_LED1, OUTPUT);
  pinMode(YELLOW_LED2, OUTPUT);
  pinMode(RED_LED1, OUTPUT);
  pinMode(RED_LED2, OUTPUT);

  pinMode(API_BUTTON, INPUT_PULLUP);
  pinMode(ALARM_BUTTON, INPUT_PULLUP);

  photoValue = analogRead(34);
  potenValue = analogRead(35);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();
}

void loop() {
  photoValue = analogRead(34);
  prevPoten = potenValue;
  potenValue = analogRead(35);

  int alarmValue = digitalRead(ALARM_BUTTON);
  int apiValue = digitalRead(API_BUTTON);

  if (lamp_working) {
    if (!time_beingSet) {
      if (apiValue == LOW && api_isPressed && api_beingSet) {
        int which_day = (potenValue / 585) + 1;
        Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
        Udp.print(which_day);
        Udp.endPacket();
        
        api_beingSet = false;
        api_isPressed = !api_isPressed;
        delay(10);
      } else if (apiValue == LOW && api_isPressed && !api_beingSet) {
        Serial.println("API is turned on/Ready to change value");
        if (api_status == false) {
          api_status = true;
          Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
          Udp.print("API");
          Udp.endPacket();
        }
        
        api_beingSet = true;
        api_isPressed = !api_isPressed;
        delay(10);
      } else if (apiValue == HIGH && !api_isPressed) {
        api_isPressed = !api_isPressed;
        delay(10);
      }
    }
  
    if (api_status && !api_beingSet) {
      if (alarmValue == LOW && alarm_isPressed) {
        Serial.println("API is turned off");
        Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
        Udp.print("CANCEL");
        Udp.endPacket();
        
        api_status = false;
        alarm_isPressed = !alarm_isPressed;
        delay(10);
      } else if (alarmValue == HIGH && !alarm_isPressed) {
        alarm_isPressed = !alarm_isPressed;
        delay(10);
      }
    } else if (!api_status && !api_beingSet) {
      if (alarmValue == LOW && alarm_isPressed && !time_beingSet) {      
        Serial.println("Time is being set");
        Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
        Udp.print("TIME");
        Udp.endPacket();
        
        time_beingSet = true;
        alarm_isPressed = !alarm_isPressed;
        delay(10);
      } else if (alarmValue == LOW && alarm_isPressed && time_beingSet && !hour_isSet && !min_isSet && !sec_isSet) {
        hour_timer = current_hour;
        Serial.println("Hour is set");
        Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
        Udp.print("MIN");
        Udp.endPacket();
  
        hour_isSet = true;
        alarm_isPressed = !alarm_isPressed;
        delay(10);
      } else if (alarmValue == LOW && alarm_isPressed && time_beingSet && hour_isSet && !min_isSet && !sec_isSet) {
        min_timer = current_min;
        Serial.println("Minute is set");
        Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
        Udp.print("SEC");
        Udp.endPacket();
  
        min_isSet = true;
        alarm_isPressed = !alarm_isPressed;
        delay(10);
      } else if (alarmValue == LOW && alarm_isPressed && time_beingSet && hour_isSet && min_isSet && !sec_isSet) {
        sec_timer = current_sec;
        Serial.println("Second is set");
        Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
        Udp.print("CONFIRM");
        Udp.endPacket();
  
        sec_isSet = true;
        alarm_isPressed = !alarm_isPressed;
        delay(10);
      } else if (alarmValue == HIGH && !alarm_isPressed) {
        alarm_isPressed = !alarm_isPressed;
        delay(10);
      }
    }
  
    if (hour_isSet & min_isSet && sec_isSet) {
      hour_isSet = false;
      min_isSet = false;
      sec_isSet = false;
      time_beingSet = false;
      
      t = now();
      alarm_isOn = true;
      Serial.println("Alarm is on");
    }
  
    if (alarm_isOn) {
      int program_sec = (hour() * 360) + (minute() * 60) + second();
      int remove_sec = (hour(t) * 360) + (minute(t) * 60) + second(t);
      int remaining_sec = program_sec - remove_sec;
      
      int real_hr =  remaining_sec / 360;
      if (real_hr > 0) {
        remaining_sec %= 360;
      }
      int real_min = remaining_sec / 60;
      if (real_min > 0) {
        remaining_sec %= 60;
      }
      int real_sec = remaining_sec;
      
      String current_time = "";
      if (real_hr < 10) {
        current_time += "0";
        current_time += String(real_hr);
      } else {
        current_time += String(real_hr);
      }
      current_time += ":";
      if (real_min < 10) {
        current_time += "0";
        current_time += String(real_min);
      } else {
        current_time += String(real_min);
      }
      current_time += ":";
      if (real_sec < 10) {
        current_time += "0";
        current_time += String(real_sec);
      } else {
        current_time += String(real_sec);
      }
      
      Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
      Udp.print(current_time);
      Udp.endPacket();
      delay(10);

      if (hour_timer == real_hr && min_timer == real_min && sec_timer == real_sec) {
        lamp_working = false;
        clear_leds();
      }
    }

    if (lamp_working) {
    
      if (time_beingSet) {
        if (!hour_isSet && !min_isSet && !sec_isSet) {
          current_hour = potenValue /178;
          prev_hour = prevPoten / 178;
          if (current_hour != prev_hour) {
            Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
            Udp.print(current_hour);
            Udp.endPacket();
            delay(10);
          }
        } else if (hour_isSet && !min_isSet && !sec_isSet) {
          current_min = potenValue / 69;
          prev_min = prevPoten / 69;
          if (current_min != prev_min) {
            Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
            Udp.print(current_min);
            Udp.endPacket();
            delay(10);
          }
        } else if (hour_isSet && min_isSet && !sec_isSet) {
          current_sec = potenValue / 69;
          prev_sec = prevPoten / 69;
          if (current_sec != prev_sec) {
            Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
            Udp.print(current_sec);
            Udp.endPacket();
            delay(10);
          }
        }
      }
  
      if (!api_status) {
        Serial.println(photoValue);
        if (photoValue < 2000) {
          clear_leds();
        } else {
          clear_blue();
          clear_green();
          clear_red();
          fill_color("YELLOW");
        }
      }
    }
  } else {
    if (alarmValue == LOW && alarm_isPressed) {
      Serial.println("Turn Lamp On");
      alarm_isPressed = !alarm_isPressed;
      lamp_working = true;
      delay(10);
    } else if (alarmValue == HIGH && !alarm_isPressed) {
      alarm_isPressed = !alarm_isPressed;
      delay(10);
    }
  }
}
