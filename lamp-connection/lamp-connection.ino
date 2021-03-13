#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define GREEN_LED1      14
#define GREEN_LED2      13
#define BLUE_LED1       12
#define BLUE_LED2       27
#define YELLOW_LED1     19
#define YELLOW_LED2     4
#define RED_LED1        18
#define RED_LED2        15

#define CONSOLE_IP "192.168.1.2"
#define CONSOLE_PORT 4210
const char* ssid = "ESP32";
const char* password = "12345678";
WiFiUDP Udp;
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

int adcValue = 0;

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

  adcValue = analogRead(34);
  Serial.println(adcValue);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  server.begin();
}

void loop() {
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  Udp.print(adcValue);
  Udp.endPacket();

  clear_leds();
  fill_color("YELLOW");

  adcValue = analogRead(34);
  Serial.println(adcValue);
  
  delay(100);
}
