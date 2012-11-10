#include <stdio.h>
#include <string>
#include "analysis.h"
#include "base.h" 

using namespace std;

int pattern::readPattern(string filename)
{
	/*
	FILE *input=fopen(filename.c_str(), "r");
	if (!input) {
		printf("Can\'t open output file %s for writeng!\n", filename.c_str());
		return -2;
	}
	len=0;
	while (!feof(input)) {
		if (len==MAX_COND_LEN) {
			printf("Pattern is too long!\n");
			return -2;
		}
		fscanf(input, "%f\t%c,%c,%f", time[len], sign[len], cond[len], dfdt[len]);
		index[len]=time2index[time[len]];
		len++;
	}
	printf("Pattern with length of %d successfuly readed.\n", len);
	fclose(input);
	return 0;*/
	return 0;
}

bool pattern::check(dataContainer& E, long long index)
{
	return false;
}

void truncData(dataContainer& E, int range)
{
	long long i;
	int j;
	int newDataLen=E.dataLen/range;
	float* newE=new float[newDataLen];
	long long offset=0;
	for(i=0;i<newDataLen; i++, offset+=range) {
		newE[i]=0;
		for (j=0; j<range; j++) {
			newE[i]+=E.E[offset+j];
		}
	}
	delete[] E.E;
	E.E=newE;
	E.dataLen=newDataLen;
}

void averageData(dataContainer& E, int range)
{
	printf ("TODO\n");
}

void rcData(dataContainer& E, float rc)
{
	printf ("TODO\n");
}
