#ifndef STATISTICS_H_INCLUDED
#define STATISTICS_H_INCLUDED

#include "base.h"
#include "unknown-with-options.h"
#include <vector>
#include <string>


struct StrikeMark {
	float t, E;
};

typedef std::vector<StrikeMark> st_vec;

class DetectedStrikesContainer : public unknownWithOptions
{
private:
	st_vec strikes;
public:
	float time_shift;
	void add(float t, float E);
	int print(string& filename);
	int printHist(string& filename);
	int printDirationHistogram(string& filename, double timestep, bool maxIntervalSpecified = false, double maxInterval = 0);
	int addReport(string& filename);
	DetectedStrikesContainer();
	~DetectedStrikesContainer();
};

#endif
