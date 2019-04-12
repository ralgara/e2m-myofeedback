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

#define SEND_UDP    0
#define SEND_SERIAL 1
#define HAPTIC_BLINK !SEND_SERIAL

/* Filter generator:
    https://www-users.cs.york.ac.uk/~fisher/cgi-bin/mkfscript

    filtertype   =   Butterworth
    passtype  =   Lowpass
    ripple  =
    order   =   4
    samplerate  =   100
    corner1   =   1

    raw alpha1    =   0.0100000000
    raw alpha2    =   0.0100000000
    warped alpha1 =   0.0100032912
    warped alpha2 =   0.0100032912
    gain at dc    :   mag = 1.112983215e+06   phase =  -0.0000000000 pi
    gain at centre:   mag = 7.869979788e+05   phase =  -1.0000000000 pi
    gain at hf    :   mag = 0.000000000e+00
*/

#define NZEROS 4
#define NPOLES 4
#define GAIN   1.112983215e+06

static float xv[NZEROS + 1], yv[NPOLES + 1];
static float filter(float value) {
  xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4];
  xv[4] = value / GAIN;
  yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4];
  yv[4] =   (xv[0] + xv[4]) + 4 * (xv[1] + xv[3]) + 6 * xv[2]
            + ( -0.8485559993 * yv[0]) + (  3.5335352195 * yv[1])
            + ( -5.5208191366 * yv[2]) + (  3.8358255406 * yv[3]);
  return yv[4];
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

void send_data_udp(int value) {
  Udp.beginPacket(target_host, target_port);
  Udp.write((value & 0xff00) >> 8);
  Udp.write(value & 0xff);
  Udp.endPacket();
}

void send_data_serial(int raw, int filtered, int feedback) {
  Serial.printf("%d %d %d\n", raw, filtered, feedback);
}

#define HAPTIC_THRESHOLD 100
#define HAPTIC_SCALE 1024
short feedback(short input) {
  if (input > HAPTIC_THRESHOLD)
    return HAPTIC_SCALE;
  else
    return 0;
}

void loop() {
  sensorValue = analogRead(sensorPin);
  short filtered = filter(sensorValue);
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
