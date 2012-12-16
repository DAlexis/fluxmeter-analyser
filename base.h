#ifndef _BASE_H_
#define _BASE_H_

#include <string>

using namespace std; 

#define MYZA_STR_COUNT		15000000

class dataContainer {
private:
	int readStdTxt(string& fileName);
	int readMyza(string& fileName, int ncols, int col); //  Temoprary :)
public:
	float time_shift;
	float *E;
	long long dataLen;
	
	int binaryInput(string& filename);
	int binaryOutput(string& filename);
	int readFresh(string& fileName, string& inpFmt);
	int textOutput(string& filename);
	
	float index2time(long long ind);
	long long time2index(double time);
	
	void setTimeShift(float shift);
	
	~dataContainer();
};

/*
struct dataContainer {
	float *E;
	long long dataLen;
};

void initTranslator(dataContainer& container);

int time2index(double time);

double index2time(long long index);*/

#endif
