#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
// Configure WiFi credentials:
#include "WiFiCredentials.h"

WiFiUDP Udp;

const char* target_host = "10.12.15.149";
int target_port = 33000;

int sensorPin = A0;
int sensorValue = 0;
int sample_rate_hz = 100;
int sample_delay_ms = 1000 / sample_rate_hz;

#define HAPTIC_THRESHOLD 300
#define HAPTIC_SCALE 1024

#define SEND_UDP    1
#define SEND_SERIAL 1
#define HAPTIC_BLINK !SEND_SERIAL

/* Filter generator:
    https://www-users.cs.york.ac.uk/~fisher/cgi-bin/mkfscript
  filtertype   =   Butterworth
  passtype  =   Lowpass
  ripple  =
  order   =   3
  samplerate  =   100
  corner1   =   1
  corner2   =
  adzero  =
  logmin  =

  Command line: /www/usr/fisher/helpers/mkfilter -Bu -Lp -o 3 -a 1.0000000000e-02 0.0000000000e+00
  raw alpha1    =   0.0100000000
  raw alpha2    =   0.0100000000
  warped alpha1 =   0.0100032912
  warped alpha2 =   0.0100032912
  gain at dc    :   mag = 3.430944333e+04   phase =  -0.0000000000 pi
  gain at centre:   mag = 2.426044003e+04   phase =  -0.7500000000 pi
  gain at hf    :   mag = 0.000000000e+00
*/

#define NZEROS 3
#define NPOLES 3
#define GAIN   3.430944333e+04

float filter(float value) {
  static float xv[NZEROS + 1];
  static float yv[NPOLES + 1];
  xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3];
  xv[3] = value / GAIN;
  yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3];
  yv[3] =   (xv[0] + xv[3]) + 3 * (xv[1] + xv[2])
            + (  0.8818931306 * yv[0]) + ( -2.7564831952 * yv[1])
            + (  2.8743568927 * yv[2]);
  return yv[3];
}

float derivative(float value) {
  static float xv;
  float xd = value - xv;
  xv = value;
  return xd * sample_rate_hz;
}

short feedback(short input) {
  if (input > HAPTIC_THRESHOLD)
    return HAPTIC_SCALE;
  else
    return 0;
}

void send_data_udp(int value) {
  Udp.beginPacket(target_host, target_port);
  Udp.write((value & 0xff00) >> 8);
  Udp.write(value & 0xff);
  Udp.endPacket();
}

void send_data_serial(
  short raw,
  short filtered,
  short feedback) {
  Serial.printf("%d %d %d\n",
                raw,
                filtered,
                feedback);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
#if SEND_SERIAL
  Serial.begin(115200);
  Serial.println();
  Serial.printf("Connecting to %s ", ssid);
#endif
#if SEND_UDP
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
#endif
  Serial.printf("Starting sampler with f=%d", sample_rate_hz);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  short filtered = filter(sensorValue);
  short d1 = derivative(filtered);
  short haptic = feedback(filtered);
#if HAPTIC_BLINK
  digitalWrite(LED_BUILTIN, haptic);
#endif
#if SEND_UDP
  send_data_udp(sensorValue);
#endif
#if SEND_SERIAL
  send_data_serial(sensorValue, filtered, haptic);
#endif
  delay(sample_delay_ms);
}
