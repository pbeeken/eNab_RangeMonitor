#include <Arduino.h>
#include <Streaming.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);

  Serial.begin(115200);
  
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  int reading = analogRead(A0);
  Serial << "Voltage" << reading << endl;

  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN, LOW);
  delay(10);
}