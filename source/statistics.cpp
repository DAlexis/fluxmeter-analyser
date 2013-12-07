#include "statistics.h"
#include <stdio.h>


strikesClass::strikesClass():time_shift(0)
{
	//printf ("Sclass constructor\n");
}

strikesClass::~strikesClass()
{
	//printf ("Sclass destructor\n");
}

void strikesClass::add(float t, float E)
{
	strikeStruct ss;
	ss.t=t; ss.E=E;
	strikes.push_back (ss);
}

int strikesClass::print(std::string& filename)
{
	FILE *output=fopen(filename.c_str(), "w");
	if (!output) {
		printf("Can\'t open file %s for writing strikes list!\n", filename.c_str());
		return -2;
	}
	
	st_vec::iterator it;
	
	for (it = strikes.begin(); it != strikes.end(); it++) {
		if (!needContinue(it->t))
			fprintf(output, "%f %f\n", it->t, it->E);
	}
	
	printf("Done.\n");
	fclose(output);
	return 0;
}

int strikesClass::printHist(string& filename)
{
	FILE *output=fopen(filename.c_str(), "w");
	if (!output) {
		printf("Can\'t open file %s for writing histogram!\n", filename.c_str());
		return -2;
	}
	
	st_vec::iterator it=strikes.begin();
	
	long int i;
	int currMin=0;
	long int time=time_shift*60*60;
	
	for (i=0; i<60*24; i++, time+=60) {
		
		currMin=0;
		
		if (it!=strikes.end())
		{
			while (it->t >= time && it->t < time+60) {
				currMin++;
				it++;
				if (it==strikes.end()) break;
			}
		}
		
		if (!needContinue(time))
			fprintf(output, "%ld %d\n", time, currMin);
	}
	
	printf("Done.\n");
	fclose(output);
	return 0;
}

int strikesClass::addReport(string& filename)
{	
	FILE *output=fopen(filename.c_str(), "w");
	if (!output) {
		printf("Can\'t open file %s for writing report!\n", filename.c_str());
		return -2;
	}
	
	fprintf(output, "%ld\n", strikes.size());
	
	fclose(output);
	return 0;
}