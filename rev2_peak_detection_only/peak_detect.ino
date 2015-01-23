#include "noiseless_samples.h"

const int numReadings = 200;


int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

const int inputPin = A0;

void setup()
{
  // initialize serial communication with computer:
  Serial.begin(9600);                   
  // initialize all the readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;

  Timer1.getAvailable().attachInterrupt(handler).start(1000);          
}


