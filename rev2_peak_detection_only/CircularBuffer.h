#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H
#define MAX_ITEMS 2000
#endif

typedef struct circularQueue_s {
    volatile int     validItems;
	volatile int     first;
    volatile int     last;
    volatile int     data[MAX_ITEMS];
} circularQueue_t;

void initializeQueue(circularQueue_t *theQueue);
int addSample(circularQueue_t *theQueue, int theItemValue);
void beginIter(circularQueue_t *theQueue);
int getSample(circularQueue_t *theQueue, int idx);

// testing code
//void printQueue(circularQueue_t *theQueue);