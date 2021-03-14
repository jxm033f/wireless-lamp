#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include <Time.h>

#define GREEN_LED1      14
#define GREEN_LED2      13
#define BLUE_LED1       12
#define BLUE_LED2       27
#define YELLOW_LED1     19
#define YELLOW_LED2     4
#define RED_LED1        18
#define RED_LED2        15

#define API_BUTTON      5
bool api_isPressed = false;
bool api_beingSet = false;
bool api_status = false;

#define ALARM_BUTTON    2
bool alarm_isPressed = false;
bool time_beingSet = false;

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

int photoValue = 0;
int potenValue = 0;

time_t t = now();

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

  photoValue = analogRead(34);
  potenValue = analogRead(35);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();
}

void loop() {
  //Serial.println("Hour: " + String(hour())+ " Minute: " + String(minute()) + " Second: " + String(second()));

  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int message_len = Udp.read(packetBuffer, 255);
    if (message_len > 0) {
      packetBuffer[message_len] = 0;
    }
  }

  if (!time_beingSet) {
    int apiValue = digitalRead(API_BUTTON);

    if (apiValue == LOW && api_isPressed && api_beingSet) {
      //read from potenciometer
      int which_day = (potenValue / 585) + 1;
      Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
      Udp.print(which_day);
      Udp.endPacket();
      
      api_beingSet = false;
      api_isPressed = !api_isPressed;
      delay(10);
    } else if (apiValue == LOW && api_isPressed && !api_beingSet) {
      api_status = true;
      api_beingSet = true;
      api_isPressed = !api_isPressed;
      delay(10);
    } else if (apiValue == HIGH && !api_isPressed) {
      api_isPressed = !api_isPressed;
      delay(10);
    }
  }

  int alarmValue = digitalRead(ALARM_BUTTON);
  if (api_status && !api_beingSet) {
    if (alarmValue == LOW && alarm_isPressed) {
      api_status = false;
      alarm_isPressed = !alarm_isPressed;
      delay(10);
    } else if (alarmValue == HIGH && !alarm_isPressed) {
      alarm_isPressed = !alarm_isPressed;
      delay(10);
    }
  } else if (!api_status && !api_beingSet) {
    if (alarmValue == LOW && alarm_isPressed) {
      
      alarm_isPressed = !alarm_isPressed;
      delay(10);
    } else if (alarmValue == HIGH && !alarm_isPressed) {
      alarm_isPressed = !alarm_isPressed;
      delay(10);
    }
  }
  
  if (api_status) {
    if (packetBuffer == "BLUE") {
      clear_yellow();
      clear_green();
      clear_red();
      fill_color(packetBuffer);
    } else if (packetBuffer == "GREEN") {
      clear_yellow();
      clear_blue();
      clear_red();
      fill_color(packetBuffer);
    } else if (packetBuffer == "YELLOW") {
      clear_green();
      clear_blue();
      clear_red();
      fill_color(packetBuffer);
    } else if (packetBuffer == "RED") {
      clear_yellow();
      clear_blue();
      clear_green();
      fill_color(packetBuffer);
    }
  } else if (!api_status) {
    //in a perfect world the leds would accommadate different values
    //since we are focusing on LOW or HIGH there can only be two options
    //i assume 1000 as the baseline with bedroom lights or a sunny day
    if (photoValue < 1200) {
      clear_leds();
    } else {
      clear_blue();
      clear_green();
      clear_red();
      fill_color("YELLOW");
    }
  }

  photoValue = analogRead(34);
  potenValue = analogRead(35);
  //Serial.println(adcValue);
}
