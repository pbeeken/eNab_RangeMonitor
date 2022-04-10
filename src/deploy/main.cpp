#include <Arduino.h>
// This code runs on an attiny with no serial connection

const int trigPin = 3;
const int echoPin = 2;

#define CONTINUOUS 5
#define INTERMITANT 3
#define OFF 0

int rangeDetect();
int limitTrigger();
void activateBuzzer(int val);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  
}

void loop() {

  int trigDist = limitTrigger();
  int distToObs = rangeDetect();

  // TODO: Explore prewarnings (repeated beeps vs continuous tones)
  if (distToObs < trigDist) {
    activateBuzzer(CONTINUOUS);
  }
  else if (distToObs < trigDist*1.2) {
    activateBuzzer(INTERMITANT);
  }
  else {
    activateBuzzer(OFF);
  }
  delay(30);
}

/**
 * activateBuzzer turns on the buzzer. On the UNO this is a hardware value so
 * it doesn't need updating, for the ATTiny, I'll have to look.
 **/
void activateBuzzer(int val) {
  switch(val) {
    case CONTINUOUS:
      analogWrite(DD5, 125); 
      break;
    case INTERMITANT:
      analogWrite(DD5, 125);
      delay(30);
      analogWrite(DD5, 0);
      break;
    default:
      analogWrite(DD5, 0);
      break;
  }
 
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