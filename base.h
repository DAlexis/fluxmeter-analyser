#ifndef _BASE_H_
#define _BASE_H_

#include <string>

using namespace std; 

class dataContainer {
public:
	float *E;
	long long dataLen;
	
	int binaryInput(string& filename);
	int binaryOutput(string& filename);
	int readFresh(string& fileName);
	int textOutput(string& filename);
	
	float index2time(long long ind);
	long long time2index(double time);
	
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