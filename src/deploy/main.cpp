#include <Arduino.h>
// This code runs on an attiny with no serial connection
#define TIME_FACTOR 1

#if F_CPU == 1000000L
#define TIME_FACTOR 1
#elif F_CPU == 8000000L
#define TIME_FACTOR 8
#endif

// maximum distance to detect in us (d/vs)
#define MAXDIST 9000L

// Pins
const int LEDPIN      = PB0;
const int EchoPIN     = PB3;
const int TriggerPIN  = PB1;
const int LimitPIN    = A2;  // Need to use A designators for analog reads
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

//  pinMode(LimitPIN, INPUT);    // Pin for reading limit
  pinMode(TriggerPIN, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(EchoPIN, INPUT);  // Sets the EchoPIN as an INPUT
  pinMode(AlertPIN, OUTPUT);
  pinMode(PB5, INPUT);

}

bool loopCtr = true; 
/**
 * continuous loop, inner part of for(;;) loop 
 **/
void loop() {

  int timeTrig = limitTrigger();
  int timeToObj = rangeDetect();

  // TODO: Explore prewarnings (repeated beeps vs continuous tones)
  if (timeToObj < timeTrig) {
    activateBuzzer(CONTINUOUS);
    //digitalWrite(LEDPIN, HIGH);
  }
  else if (timeToObj < timeTrig*12/10) {
    activateBuzzer(INTERMITANT);
  }
  else {
    activateBuzzer(OFF);
    //digitalWrite(LEDPIN, LOW);
  }
  delay(20);
}

/**
 * activateBuzzer turns on the buzzer. On the UNO this is a hardware value so
 * it doesn't need updating, for the ATTiny, I'll have to look.
 **/
void activateBuzzer(int val) {
  switch(val) {
    case CONTINUOUS:
      analogWrite(AlertPIN, 255); 
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

//   digitalWrite(LEDPIN, HIGH);
//   delayMicroseconds(reading*2);
//   digitalWrite(LEDPIN, LOW);

  // Convert from the max range on an analog input to the value corresponding to a distance away in cm  
  return map(reading, 0L, 1023L, 0L, MAXDIST);
}

/**
 * rangeDetect determines the range from the sonic sensor to an obstruction
 * @returns the range in cm
 **/
int rangeDetect() {
  // Clears the trigPin condition
  digitalWrite(TriggerPIN, LOW);
  delayMicroseconds(2/TIME_FACTOR);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TriggerPIN, HIGH);
  delayMicroseconds(10/TIME_FACTOR);
  digitalWrite(TriggerPIN, LOW);
  // Reads the EchoPIN, returns the sound wave travel time in microseconds
  uint32_t duration = pulseInLong(EchoPIN, HIGH, 2*MAXDIST)*TIME_FACTOR;
  // Calculating the distance
  int distance = duration / 2; // Keep to integer math 4m = 11764 us
  // Displays the distance on the Serial Monitor
  return distance;
}