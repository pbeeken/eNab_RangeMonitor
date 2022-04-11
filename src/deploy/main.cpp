#include <Arduino.h>
// This code runs on an attiny with no serial connection

// Pins
const int LEDPIN      = PB0;
const int EchoPIN     = PB3;
const int TriggerPIN  = PB1;
const int LimitPIN    = PB4;
const int AlertPIN    = PB2;

// States
const int OFF         = 0;
const int CONTINUOUS  = 3;
const int INTERMITANT = 5;

int rangeDetect();
int limitTrigger();
void activateBuzzer(int val);

/**
 * setup prior to loop, initialization 
 **/
void setup() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  pinMode(LimitPIN, INPUT);    // Pin for reading limit
  pinMode(TriggerPIN, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(EchoPIN, INPUT);  // Sets the EchoPIN as an INPUT
  pinMode(AlertPIN, OUTPUT);

}

bool loopCtr = true; 
/**
 * continuous loop, inner part of for(;;) loop 
 **/
void loop() {

  int trigDist = limitTrigger();
  int distToObs = rangeDetect();

  // TODO: Explore prewarnings (repeated beeps vs continuous tones)
  if (distToObs < trigDist) {
    activateBuzzer(CONTINUOUS);
    digitalWrite(LEDPIN, HIGH);
  }
  else if (distToObs < trigDist*1.2) {
    activateBuzzer(INTERMITANT);
  }
  else {
    activateBuzzer(OFF);
    digitalWrite(LEDPIN, LOW);
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
      analogWrite(AlertPIN, 125); 
      break;
    case INTERMITANT:
      analogWrite(AlertPIN, 125);
      delay(30);
      analogWrite(AlertPIN, 0);
      break;
    default:
      analogWrite(AlertPIN, 0);
      break;
  }
 
}

/**
 * limitTrigger reads the voltage on a Pot and converts 
 * to the trigger value in cm that would sound the siren
 * @returns distance in cm
 **/
int limitTrigger() {
  int reading = analogRead(LimitPIN);
  // Convert from the max range on an analog input to the value corresponding to a distance away in cm  
  return map(reading, 0L, 1024L, 0L, 400L);
}

/**
 * rangeDetect determines the range from the sonic sensor to an obstruction
 * @returns the range in cm
 **/
int rangeDetect() {
  // Clears the trigPin condition
  digitalWrite(TriggerPIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TriggerPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TriggerPIN, LOW);
  // Reads the EchoPIN, returns the sound wave travel time in microseconds
  uint32_t duration = pulseIn(EchoPIN, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  return distance;
}