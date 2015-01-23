#include "SimpleTimer.h" // millis() polling timer for digital output
#include "CircularBuffer.h" // datastructure for storing samples
#include "DueTimer.h" // hardware timer for doing ansynchronous sampling

SimpleTimer outputPulseTimer; // used for timing the output pulses
circularQueue_t sampleQueue;  // used for storing the samples. circular b/c new overwrite old

// we need a range of 1-4 Hz possible, with greatest accuracy around 1-3
// at a 5ms period, we will need 500 samples
const int SAMPLE_PERIOD = 2; // (ms)
const int ANALOG_INPUT_PIN = 0;
const int DIGITAL_OUTPUT_PIN = 23;

volatile int newSamples = 0;

// Arduino Functions
void setup() {
  initializeQueue(&sampleQueue);
  analogReadResolution(12); // The arduino Due has 12 bit ADCs, so this is fastest
  Serial.begin(115200);
  pinMode(ANALOG_INPUT_PIN, INPUT);
  Timer3.attachInterrupt(sample).start(SAMPLE_PERIOD * 1000); // Hardware timer every 5ms triggers sampling interrupt service routine
  outputPulseTimer.setInterval(1000, triggerSignal); // polling (non-interrupt) timer that triggers the output pulse
}



void loop() {
  outputPulseTimer.run();
  // we don't want to sample while running autoCorrelate frequency
  // because modifying the source data during autocorrelation could cause problems
  while(newSamples<2000){
    outputPulseTimer.run();
  } // just sample
  Timer3.stop(); // stop sampling so that the data structure will be stable
  int period = autoCorrelateFrequency(&sampleQueue);
  Serial.println(period);
  printQueue(&sampleQueue);
  outputPulseTimer.changeTimerInterval(0, period);
  newSamples = 0;
  Timer3.start();
}


void sample() {
  // gets a sample (from analog input 0) and puts that sample in teh queue
  int sampleVal = analogRead(ANALOG_INPUT_PIN);
  addSample(&sampleQueue, sampleVal);
  newSamples += 1;
}

void triggerSignal() {
  // sets digital output pin HIGH to produce rising edge of MRI trigger signal
  digitalWrite(DIGITAL_OUTPUT_PIN, HIGH);
  outputPulseTimer.setTimeout(30, fallingEdge);
}

void fallingEdge() {
  // sets digital output pin LOW to produce falling edge to MRI trigger signal
  digitalWrite(DIGITAL_OUTPUT_PIN, LOW);
}


int mean(circularQueue_t *samples) {
  // returns the arithmetic mean of the sample queue
  int ii;	
	int sum = 0;
	for(ii=0; ii<samples->validItems; ii++) {
    sum += getSample(samples, ii);
  }
  return sum / (ii-1);
}

int autoCorrelateFrequency(circularQueue_t *samples) {	
  // frequency autocorrelation based on
  // http://www.instructables.com/id/Reliable-Frequency-Detection-Using-DSP-Techniques/
  // http://stackoverflow.com/questions/435533/detecting-the-fundamental-frequency
  // http://en.wikipedia.org/wiki/Autocorrelation

  int samplemean = mean(samples);
  int sum = 0;
  int prevSum = 0;
  int peakThreshold = 0;
  int ii;
  int kk; // scale factor for normalization over different data set sizes
  char pd_state = 0;
  int estimatePeriod = 0;  // 0 results in inf
  // Autocorrelation

  for (ii=0; ii < samples->validItems; ii++) {
    outputPulseTimer.run();
    prevSum = sum;
    sum = 0;

    for (kk=0; kk <samples->validItems - ii; kk++) {
      sum += (getSample(samples, kk) - samplemean) * (getSample(samples, kk+ii) - samplemean);
    }
    //sum /= kk ;  //this should be kk-1, but if so the last point will result in divide by zero

    // Peak Detect State Machine
    // 0: initial, set threshold
    // 1: detect positive slope
    // 2: detect negative slope
    // 3: peak found
    if (pd_state == 0) {
      peakThreshold = sum/2;
      pd_state = 1;
    }
    else if (pd_state == 1 && (sum > peakThreshold) && (sum - prevSum) > 0) {
      pd_state = 2;
    }
    else if (pd_state == 2 && (sum - prevSum) <= 0) {
      estimatePeriod = ii;
      return estimatePeriod * SAMPLE_PERIOD;
    }
  }
}