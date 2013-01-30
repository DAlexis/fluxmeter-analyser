#ifndef _BASE_H_
#define _BASE_H_

#include <string>
#include "unknown-with-options.h"

using namespace std; 

#define MYZA_STR_COUNT		20000000

class dataContainer : public unknownWithOptions
{
private:
	int readEFM(string& fileName);
	int readDAT(string& fileName, int ncols, int col); //  Temoprary :)	
public:
	float time_shift;
	float *E;
	long long dataLen;
	int text_out_step;
	
	int binaryInput(string& filename);
	int binaryOutput(string& filename);
	int readFresh(string& fileName, string& inpFmt);
	int textOutput(string& filename);
	
	float index2time(long long ind);
	long long time2index(double time);
	float index2timeInterval(long long ind);
	long long timeInterval2index(double time);
	
	
	void setTimeShift(float shift);
	 
	dataContainer();
	~dataContainer();
};

#endif
