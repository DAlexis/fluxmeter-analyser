#include "statistics.h"
#include <stdio.h>
#include <string.h>


DetectedStrikesContainer::DetectedStrikesContainer():time_shift(0)
{
	//printf ("Sclass constructor\n");
}

DetectedStrikesContainer::~DetectedStrikesContainer()
{
	//printf ("Sclass destructor\n");
}

void DetectedStrikesContainer::add(float t, float E)
{
	StrikeMark ss;
	ss.t=t; ss.E=E;
	strikes.push_back (ss);
}

int DetectedStrikesContainer::print(std::string& filename)
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

int DetectedStrikesContainer::printHist(string& filename)
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
	
	fclose(output);
	printf("Done.\n");
	return 0;
}

int DetectedStrikesContainer::printDirationHistogram(string& filename, double timestep, bool maxIntervalSpecified, double maxInterval)
{
	FILE *output=fopen(filename.c_str(), "w");
	if (!output) {
		printf("Can\'t open file %s for writing histogram!\n", filename.c_str());
		return -2;
	}
	// Calculating histogram
	int *histogram = NULL;
	unsigned int count = 0;
	// Determining max interval
	auto it = strikes.begin();
	double lastTime = it++->t;
	double maxTimeInterval = 0;
	for (; it != strikes.end(); it++)
	{
		double timeInterval = it->t - lastTime;
		lastTime = it->t;
		if (maxTimeInterval < timeInterval) maxTimeInterval = timeInterval;
	}
	count = maxTimeInterval/timestep + 1;
	if (maxIntervalSpecified)
	{
		unsigned int maxCount = maxInterval/timestep;
		if (maxCount < count)
			count = maxCount;
	}
	
	histogram = new int[count];
	memset(histogram, 0, sizeof(int)*count);
	it = strikes.begin();
	lastTime = it++->t;
	for (; it != strikes.end(); it++)
	{
		double timeInterval = it->t - lastTime;
		lastTime = it->t;
		unsigned int index = timeInterval / timestep;
		if (index < count)
			histogram[index]++;
	}
	
	for (unsigned int i=0; i<count; i++)
	{
		fprintf(output, "%lf %d\n", i*timestep, histogram[i]);
	}
	
	if (histogram) delete[] histogram;
	fclose(output);
	printf("Done.\n");
	return 0;
}

int DetectedStrikesContainer::addReport(string& filename)
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
