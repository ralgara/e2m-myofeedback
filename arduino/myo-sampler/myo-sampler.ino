
int sensorPin = A0;
int sensorValue = 0;
int period = 20;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(57600);
  Serial.println("Start");
}

void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  delay(period);
}
