#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include "base.h"
#include <vector>
#include <string>

struct strikeStruct {
	float t, E;
};

class strikesClass {
private:
	std::vector<strikeStruct> strikes;
	
public:
	void add(float t, float E);
	int print(string& filename);
};

#endif
