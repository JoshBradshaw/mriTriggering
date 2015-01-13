#include <TimedAction.h>

int triggerPeriod = 1000; // period between trigger pulses (ms)
TimedAction triggerTiming = TimedAction(triggerPeriod, trigger);

const int samplePeriod = 200; // period between input sampling (ms)
TimedAction sampleTiming = TimedAction(samplePeriod, sample);

const int mriTriggerPin = 23; // output pin for triggering TTL square wave
bool trigger_state = LOW;
unsigned long falling_edge_time;

void setup() {
  // put your setup code here, to run once:
  analogReadResolution(12); // this board has 12 bit hardware ADCs, so this will be fastest
  Serial.begin(9600);
  pinMode(mriTriggerPin, OUTPUT);
}


void trigger() {
  if (trigger_state == HIGH) { // error checking
    Serial.print("Trigger State Incorrect, should be HIGH");
  }
  trigger_state = HIGH;
  digitalWrite(mriTriggerPin, trigger_state);
  // produce a 5ms pulse
  falling_edge_time = millis() + 5;
  Serial.println(falling_edge_time);
}

void sample() {
  
}

void loop() {
    // checks whether there it's time to make another rising edge
    // if there's a lot of latency in the program, it has to be placed comfortably between the rising edges
    triggerTiming.check();
    sampleTiming.check();
    // enforce 5ms pulses 
    // (this doesn't need to be super exact b/c it doesn't affect the timing of the rising edges)
    if (trigger_state == HIGH && millis() > falling_edge_time){
      trigger_state = LOW;
      digitalWrite(mriTriggerPin, trigger_state);
    }
}
