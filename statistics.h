#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include "base.h"
#include <vector>

struct strikeStruct {
	float t, E;
};

class strikesClass {
private:
	vector<strikeStruct> strikes;
	
public:
	void add(float t, float E);
};

#endif
