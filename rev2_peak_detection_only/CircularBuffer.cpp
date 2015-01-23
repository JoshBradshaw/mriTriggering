#include <stdio.h>
#include <stdlib.h>
#include "CircularBuffer.h"

// based on: http://www.xappsoftware.com/wordpress/2012/09/27/a-simple-implementation-of-a-circular-queue-in-c-language/
// Modifed by Josh Bradshaw to make it overwrite old samples when the queue fills up, even if they haven't been used

void initializeQueue(circularQueue_t *theQueue) {
    int i;
    theQueue->validItems  =  0;
    theQueue->first       =  0;
    theQueue->last        =  0;
    return;
}

int addSample(circularQueue_t *theQueue, int theItemValue) {
  if(theQueue->validItems==MAX_ITEMS) {
      theQueue->data[theQueue->last] = theItemValue;
      theQueue->first = (theQueue->first+1)%MAX_ITEMS;
    } else {
      theQueue->validItems++;
      theQueue->data[theQueue->last] = theItemValue;    
    }
    theQueue->last = (theQueue->last+1)%MAX_ITEMS;
}

int getSample(circularQueue_t *theQueue, int idx) {
	return theQueue->data[(theQueue->first + idx)%MAX_ITEMS];
}

//// Testing code (commented out to save Arduino storage)
// void printQueue(circularQueue_t *theQueue) {
//   for(int ii=0; ii<theQueue->validItems; ii++) {
//     int val = getSample(theQueue, ii);
//     printf("sample [%d]: %d \n", ii, val);
//   }
// }


//int main(void) {
//	circularQueue_t sampleQueue;
//	initializeQueue(&sampleQueue);
//	
//	for(int ii=0; ii<MAX_ITEMS+1; ii++) {
//		addSample(&sampleQueue, ii);
//	}
//	
//	for(int ii=0; ii<sampleQueue.validItems; ii++) {
//		int val = get(&sampleQueue, ii);
//		printf("val: %d \n", val);
//	}
//}
