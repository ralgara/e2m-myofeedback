#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

WiFiUDP Udp;

const char* ssid = "****";
const char* password = "****";

int sensorPin = A0;
int sensorValue = 0;
int period = 20;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}


void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  Udp.beginPacket("10.12.15.149", 33000);
  //Udp.write(&sensorValue, 2);
  Udp.write((sensorValue & 0xff00) >> 8);
  Udp.write(sensorValue & 0xff);
  Udp.endPacket();
  delay(period);
}
