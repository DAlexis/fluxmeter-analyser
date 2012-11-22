#ifndef _ANALYSIS_H_
#define _ANALYSIS_H_

#include <string>
#include "base.h"

using namespace std;

#define MAX_COND_LEN		100

#define AM_SIMPLE			1
#define AM_PATTERN			2

class pattern {
private: 
	float time[MAX_COND_LEN];
	float dfdt[MAX_COND_LEN];
	char cond[MAX_COND_LEN]; // <, >
	char sign[MAX_COND_LEN]; // s - same, d - different, e - every, +/-
	char strength[MAX_COND_LEN]; // a - all the time, o - >=one time
	
	int len;
	float dt;
	
	float preTime, pre_dfdt;
	float stepTime, stepValue;
	float reverseTime;
	char needReverse; 
	
	bool check(dataContainer& E, long long index, bool print);
	bool simpleCheck(dataContainer& E, long long index, bool print);
public:	
	int readPattern(string filename);
	int readSimplePattern(string filename);
	pattern(): len(0), dt(0.05) {}
	void setDt(float new_dt);
	int outStrikes(dataContainer& E, string& filename, char method);
	int outStrikes(dataContainer& E, string& filename, char method, float tr_beg, float tr_end);
};

void truncData(dataContainer& E, int range);
void averageData(dataContainer& E, int range);
void rcData(dataContainer& E, float rc);
void nlF1(dataContainer& E, float par1, float par2);
void quantumFilter(dataContainer& E, float quantum);

#endif
	
