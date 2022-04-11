#include <Arduino.h>
// This code runs on a full scale Arduino with a serial connection
#include <Streaming.h>

// Pins
const int LEDPIN      = LED_BUILTIN;
const int EchoPIN     = DD2;
const int TriggerPIN  = DD3;
const int LimitPIN    = A1;
const int AlertPIN    = DD5;

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
  pinMode(LimitPIN, INPUT);    // Pin to read limit
  pinMode(TriggerPIN, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(EchoPIN, INPUT);  // Sets the EchoPIN as an INPUT
  pinMode(LEDPIN, INPUT);

  Serial.begin(115200);
}

/**
 * continuous loop, inner part of for(;;) loop 
 **/
void loop() {

  int trigDist = limitTrigger();
  int distToObs = rangeDetect();
  Serial << "Voltage (" << trigDist << ") - (" << distToObs << ")" << endl;

  // TODO: Explore prewarnings (repeated beeps vs continuous tones)
  if (distToObs < trigDist) {
    digitalWrite(LEDPIN, HIGH);
    activateBuzzer(CONTINUOUS);
  }
  else if (distToObs < trigDist*1.15) {
    activateBuzzer(INTERMITANT);
  }
  else {
    digitalWrite(LEDPIN, LOW);
    activateBuzzer(OFF);
  }
  delay(30);
}

/**
 * activateBuzzer turns on the buzzer. On the UNO this is a hardware value so
 * it doesn't need updating, for the ATTiny, I'll have to look.
 **/
void activateBuzzer(int val) {
  if(val == CONTINUOUS) {
    analogWrite(AlertPIN, 125);
    } 
  else if (val == INTERMITANT)  {
    analogWrite(AlertPIN, 125);
    delay(30);
    analogWrite(AlertPIN, 0);
    }
  else {
    analogWrite(AlertPIN, 0);
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