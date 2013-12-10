#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#include "base.h"
#include "analysis.h"
#include "statistics.h"
#include "cmdline-parser.h"
#include "unknown-with-options.h"

class systemClass
{
private:
	dataContainer data;
	jobList job;
	pattern patt;
	DetectedStrikesContainer strikes;
public:
	int getCmdline(int argc, char** argv);
	int input();
	void process();
	int output();
	
	int configDetector();
	void detect();
};


#endif
