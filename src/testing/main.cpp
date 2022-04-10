#include <Arduino.h>
#include <Streaming.h>

const int trigPin = 3;
const int echoPin = 2;

int rangeDetect();
int limitTrigger();


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT


  Serial.begin(115200);
  
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  int trigDist = limitTrigger();
  int distToObs = rangeDetect();
  Serial << "Voltage (" << trigDist << ") - (" << distToObs << ")" << endl;

  // TODO: Operate asynchronously using old tricks
  if (distToObs < trigDist) {
    analogWrite(DD5, 125);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
    delay(20);
  } 
  else {
    analogWrite(DD5, 0);
  }


  delay(50);
}

/**
 * limitTrigger reads the voltage on a Pot and converts 
 * to the trigger value in cm that would sound the siren
 * @returns distance in cm
 **/
int limitTrigger() {
  int reading = analogRead(A0);
  // Convert from the max range on an analog input to the value corresponding to a distance away in cm  
  return map(reading, 0L, 1024L, 0L, 400L);
}

/**
 * rangeDetect determines the range from the sonic sensor to an obstruction
 * @returns the range in cm
 **/
int rangeDetect() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  uint32_t duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  return distance;
}