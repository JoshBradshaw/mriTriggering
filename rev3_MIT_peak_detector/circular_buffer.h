const int BUFFER_LEN = 32; // determines how many samples will be stored at a time

class cicularBuffer {
	// old items overwrite new items
	// item 0 is the oldest, item BUFFER_LEN -1 is the newest
	public:
		cicularBuffer() {
			for(int ii=0; ii < BUFFER_LEN; ii++) {
				addSample(0);
			}
		}
	private:
		// volatile so that Arduino won't store these values in registers
		// which would break the interrupt service routines
		volatile int buff[BUFFER_LEN];
		volatile int validItems = 0;
		volatile int first = 0;
		volatile int last = 0;
	public:
		volatile int& operator[] (const int nIndex) {
			// python style indexing, negative indices count back from the end
			if (nIndex > -1)  {
				return buff[(first + nIndex)%BUFFER_LEN];
			}		 
		    else {
		    	return buff[(first + validItems + nIndex)%BUFFER_LEN];
			}
		}
		void addSample(const int newSample) {
			if(validItems==BUFFER_LEN) {
		      buff[last] = newSample;
		      first = (first+1)%BUFFER_LEN;
		    } else {
		      validItems++;
		      buff[last] = newSample;    
		    }
		    last = (last+1)%BUFFER_LEN;
		}
};
