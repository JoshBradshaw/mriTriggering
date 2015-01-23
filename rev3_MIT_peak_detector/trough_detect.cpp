#include "csv.h"
#include <iostream>
#include <fstream>

// from: http://www.schwietering.com/jayduino/filtuino/index.php?characteristic=ch&passmode=lp&order=2&chebrip=-3&usesr=usesr&sr=250&frequencyLow=16&noteLow=&noteHigh=&pw=pw&calctype=long&bitres=12&brutalShift=brutalShift&run=Send
// fixed point arithmetic, because the Arduino doesn't have native floating point instructions
//Band pass butterworth filter order=1 alpha1=0.0046666666666667 alpha2=0.023333333333333 

class slopesum
{
	public:
		slopesum()
		{
			for(int i=0; i < 32; i++)
				v[i]=0;
		}
	private:
		short v[32];
	public:
		short step(short x)
		{
			for(int i = 1; i < 32; i++){
				v[i-1] = v[i];
			}
			
			v[31] = x;
			
			int slope_sum = 0;
			int slope;
			for(int i=1; i < 32; i++) {
				slope = v[i] - v[i-1];
				if (slope > 0) {
					slope_sum += slope;
				}
			}
			return slope_sum;
		}
};		

int main(){
  slopesum lowerpassfilter;

  io::CSVReader<2> in("test_data/unfiltered_values.csv");
  in.read_header(io::ignore_extra_column, "time", "bp");
  double time; int bp;
  
  std::ofstream myfile;
  myfile.open ("test_data/filtered_values.csv");
  
  while(in.read_row(time, bp)){
  	int filtered_val = lowerpassfilter.step(bp);
    myfile << time << "," << filtered_val << "\n";
  }
  myfile.close();
}
