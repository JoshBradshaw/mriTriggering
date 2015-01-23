#include <stdio.h>
#include <stdlib.h>
#include "noiseless_samples.h"

const int NUM_SAMPLES = 1999;

void basicPeakDetector(int sampleArray[]) {
	for(int ii=2; ii < NUM_SAMPLES; ii++) {
		if (sampleArray[ii-2] >= sampleArray[ii] && sampleArray[ii] > sampleArray[ii-1]){
			printf("Peak Detected [%d]: %d \n", ii, sampleArray[ii-1]);
		}
	}
}
// as was expected, that didn't work particuarly well because of the little perturbations between values

int mean(int nums[], int len) {
	int ii = 0;
	int sum = 0;
	for(int ii=0; ii<len; ii++){
		sum += nums[ii];
	}
	return sum / len;
}

void bucketAveragePeakDetector(int sampleArray[]) {
	int ii;
	const int BUCKET_SIZE = 24; 
	// track three sums at a time
	// initialized strictly increasing to avoid false positive on the first three buckets
	int sum0 = 16000 * 3;
	int sum1 = 12000 * 3;
	int sum2 = 8000 * 3; 
	
	int bucketSum = sampleArray[0];
	
	for(ii = 1; ii<NUM_SAMPLES; ii++) {
		if(ii % BUCKET_SIZE == 0) {
			sum0 = sum1;
			sum1 = sum2;
			sum2 = bucketSum; // no need to avg b/c bucket size is a constant
			bucketSum = sampleArray[ii];
			//printf("ii: %d, sum0: %d, sum1: %d, sum2: %d \n", ii, sum0, sum1, sum2);
			if(sum1 >= sum0 && sum1 >= sum2) {
				printf("ii: %d, sum0: %d, sum1: %d, sum2: %d \n", ii, sum0, sum1, sum2);
				printf("found a peak in the vicinity of: %d \n", ii - BUCKET_SIZE);
			}
		} else {
			bucketSum += sampleArray[ii];
		}
	}
}

// this works pretty wel actually
// its not real time, but its latency might be consistent enough?

int main(void) {
	//basicPeakDetector(samples);
	bucketAveragePeakDetector(samples);
	
	system("PAUSE");
}
