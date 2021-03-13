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

#define API_BUTTON      39
bool api_isPressed = false;
bool api_beingSet = false;

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

  pinMode(API_BUTTON, INPUT);

  photoValue = analogRead(34);
  potenValue = analogRead(35);
  
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();
}

void loop() {
  //Serial.println("Hour: " + String(hour())+ " Minute: " + String(minute()) + " Second: " + String(second()));
  
//  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
//  //Udp.print(potenValue);
//  Udp.endPacket();

  if (!time_beingSet) {
    if (API_BUTTON == LOW && api_isPressed && api_beingSet) {
      //read from potenciometer
      int which_day = potenValue / 585;
      Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
      Udp.print("Day:" + which_day);
      Udp.endPacket();

      Serial.println(which_day);
  
      api_beingSet = false;
      api_isPressed = !api_isPressed;
    } else if (API_BUTTON == LOW && api_isPressed && !api_beingSet) {
      api_beingSet = true;
      api_isPressed = !api_isPressed;

      Serial.println("WORKING?");
    } else if (API_BUTTON == HIGH && !api_isPressed) {
      Serial.println("HELLO?");
      api_isPressed = !api_isPressed;
    }
  }
  
  if (!api_beingSet) {
    if (ALARM_BUTTON == LOW && alarm_isPressed) {
      

      alarm_isPressed = !alarm_isPressed;
    } else if (ALARM_BUTTON == HIGH && !alarm_isPressed) {
      alarm_isPressed = !alarm_isPressed;
    }
  }
  
  if (!api_beingSet && !time_beingSet) {
    //in a perfect world the leds would accommadate different values
    //since we are focusing on LOW or HIGH there can only be two options
    //i assume 1000 as the baseline with bedroom lights or a sunny day
    if (photoValue < 1200) {
      clear_leds();
    } else {
      fill_color("YELLOW");
    }
  }

  photoValue = analogRead(34);
  potenValue = analogRead(35);
  //Serial.println(adcValue);
}
