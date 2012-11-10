#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

#include <string>
#include "base.h"

using namespace std;

#define MAX_COND_LEN		100
/*
class pattern {
private: 
	float time[MAX_COND_LEN];
	float dfdt[MAX_COND_LEN];
	char cond[MAX_COND_LEN]; // <, >
	char sign[MAX_COND_LEN]; // s - same, d - different, e - every, +/-
	char freq[MAX_COND_LEN]; // a - all the time, o - >=one time
	int index[MAX_COND_LEN];
	int len;
	//float* E;
public:	
	int readPattern(string filename);
	bool check(dataContainer& E, long long index);
	pattern(): len(0) {}
	//pattern(float* newE): E(newE) {}
};*/

void truncData(dataContainer& E, int range);
void averageData(dataContainer& E, int range);
void rcData(dataContainer& E, float rc);

#endif
