#include <stdio.h>
#include <stdlib.h>
#include "sample.h"


void setup()
{
  Serial.begin(9600);
}

float autoCorrelateFrequency(int * sample, int len, float sampleFreq)
{
  long sum = 0;
  long sum_old = 0;
  int thresh = 0,i,k;
  char pd_state = 0;
  int period = 0;  // 0 results in inf
  // Autocorrelation
  for (i=0; i < len; i++)
  {
    sum_old = sum;
    sum = 0;

    for (k=0; k <len-i; k++)
    {
      sum += sample[k] * sample[k+i];
    }
    sum /= k ;  //this should be k-1, but if so the last point will result in divide by zero
    // fprintf(fp,"%ld\n",sum);
    // Peak Detect State Machine
    // 0: initial, set threshold
    // 1: detect positive slope
    // 2: detect negative slope
    // 3: peak found
    if (pd_state == 0)
    {
      thresh = sum / 2;
      pd_state = 1;
    }
    else if (pd_state == 1 && (sum > thresh) && (sum - sum_old) > 0)
    {
      pd_state = 2;
    }
    else if (pd_state == 2 && (sum - sum_old) <= 0)
    {
      period = i;
      pd_state = 3;
    }
  }
  return sampleFreq/period;
}

void loop()
{
  float fr = autoCorrelateFrequency(rawData, sizeof(rawData), 5);
  Serial.print("\n");
  Serial.print("freq: ");
  Serial.print(fr);
  Serial.print("\n");
}

