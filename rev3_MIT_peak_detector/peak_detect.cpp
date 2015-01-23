#include "csv.h"
#include "circular_buffer.h"
#include <iostream>
#include <fstream>


class filter {
	public:
		filter() {
			v[0]=0;
			v[1]=0;
		}
	private:
		volatile int v[2];
	public:
		int step(const int x) {
			v[0] = v[1];
			int tmp = ((((x * 541675) >>  2)	//= (   2.5829055597e-1 * x)
				+ ((v[0] * 1013803) >> 2)	//+(  0.4834188881*v[0])
				)+262144) >> 19; // round and downshift fixed point /524288

			v[1]= tmp;
			return(v[0] + v[1]); // 2^
		}
};

class slopesum {
	public:
		slopesum(void) {}
	private:
		int slope_sum = 0;
		cicularBuffer sampleBuffer;
	public:
		int step(const int x) {	
			// the result of this recursive calculation is equivilant to adding all of the positive
			// slopes. we just subtract the contribution of the one we're taking away, and add
			// the contribution of the one we're adding
			int old_slope = sampleBuffer[1] - sampleBuffer[0];
			if (old_slope > 0) {
				slope_sum -= old_slope;
			}
			
			int new_slope = x - sampleBuffer[-1];
			
			if (new_slope > 0) {
				slope_sum += new_slope;
			}
							
			sampleBuffer.addSample(x);
			return slope_sum;
		}
};

class peakDetect {
	// algorithm: Time t is a peak if (y(t) - y(t-r)) > m && (y(t) - y(t+r)) > m (Calc 101)
	// to make this a causal system, we'll operate with a delay of r samples to give
	// Time t-r is a peak if (y(t-r) - y(t-2r)) > m && (y(t-r) - y(t)) > m
	public:
		peakDetect() {}
	private:
		cicularBuffer sb;
		const int r=6;
		const int m=2;
		const int refractory_period = 50; // 200ms at 250Hz sampling
		int refactory_counter = 0;
	public:
		bool isPeak(int x) {	
			sb.addSample(x);
			
			if (refactory_counter > refractory_period && (sb[-1-r] - sb[-1-2*r]) > m && (sb[-1-r] - sb[-1]) > m) {
				refactory_counter = 0;
				return(true);
			} else {
				refactory_counter++;
				return(false);
			}
		}
};		

int main(){
  //slopesum lowerpassfilter;
  filter lowerpassfilter;
  slopesum ssf;
  peakDetect pd;

  io::CSVReader<2> in("test_data/unfiltered_values.csv");
  in.read_header(io::ignore_extra_column, "time", "bp");
  double time; int bp;
  
  std::ofstream myfile;
  myfile.open ("test_data/filtered_values.csv");
  

  while(in.read_row(time, bp)){
  	//int filtered_val = lowerpassfilter.step(bp);
  	int ssf_filtered_val = ssf.step(bp);
  	bool peak = pd.isPeak(ssf_filtered_val);
    myfile << time << "," << ssf_filtered_val << "," << peak << "\n";
  }
  myfile.close();
}
