#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include "base.h"
#include <vector>
#include <string>

struct strikeStruct {
	float t, E;
};

typedef std::vector<strikeStruct> st_vec;

class strikesClass {
private:
	st_vec strikes;
	
public:
	void add(float t, float E);
	int print(string& filename);
	int printHist(string& filename);
};

#endif