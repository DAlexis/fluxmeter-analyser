#include <stdio.h>
#include <string>
#include "analysis.h"
#include "base.h" 

using namespace std;

int m_sign(float x)
{
	if (x<0) return -1;
	else return 1;
}

float m_abs(float x)
{
	if (x<0) return -x;
	else return x;
}

int pattern::readPattern(string filename)
{
	
	FILE *input=fopen(filename.c_str(), "r");
	if (!input) {
		printf("Can\'t open file %s for reading pattern!\n", filename.c_str());
		return -2;
	}
	len=0;
	while (!feof(input)) {
		if (len==MAX_COND_LEN) {
			printf("Pattern is too long!\n");
			return -2;
		}
		fscanf(input, "%f\t%c,%c,%c,%f", &(time[len]), &(sign[len]), &(cond[len]), &(strength[len]), &(dfdt[len]));
		//printf("time=%f, %c,%c,%c, val=%f\n", (time[len]), (sign[len]), (cond[len]), (strength[len]), (dfdt[len]));
		len++;
	}
	printf("Pattern with length of %d successfuly readed.\n", len);
	fclose(input);
	return 0;
}

bool pattern::check(dataContainer& E, long long index)
{
	int di=E.time2index(dt);
	if (di==0) {
		di=1;
		dt=E.index2time(1);
	}
	long long i=index;
	float df_dt=(E.E[i+di]-E.E[i])/dt;
	int last_sign=1;
	int stage=0;
	int in_stage=0;
	int count_in_stage;
	printf ("\n%lld Starting detection, df_dt=%f, dt=%f, di=%d\n", index, df_dt, dt, di);
	
	for (stage=0; stage<len; stage++){

		count_in_stage=E.time2index(time[stage]);
		printf("stage=%d, time[stage]=%f, count_in_stage=%d\n",stage, time[stage],count_in_stage);
		
		for (in_stage=0; in_stage<count_in_stage; in_stage++, i++) {
			
			df_dt=(E.E[i+di]-E.E[i])/dt;
			printf ("  in_stage=%d, sign[stage]=%c, m_sign(df_dt)=%d\n", in_stage, sign[stage], m_sign(df_dt));
			switch(sign[stage])
			{
				case '+': if (m_sign(df_dt)==-1) return false; break;
				case '-': if (m_sign(df_dt)==1) return false; break;
				case 's': if (m_sign(df_dt)!=last_sign) return false; break;
				case 'd': if (m_sign(df_dt)==last_sign) return false; break;
			}
			
			switch(cond[stage]){
				case '<': if (m_abs(df_dt)>dfdt[stage]) return false; break;
				case '>': if (m_abs(df_dt)<dfdt[stage]) return false; break;
			}
		}
		
		last_sign=m_sign(df_dt);
		printf("stage %d done\n", stage);
	}
	return true;
}

int pattern::outStrikes(dataContainer& E, string& filename)
{
	FILE *output=fopen(filename.c_str(), "w");
	if (!output) {
		printf("Can\'t open output file %s for writing!\n", filename.c_str());
		return -2;
	}
	long long i;
	int j;
	float totalTime=0;
	for (j=0; j<len; j++) {
		totalTime+=time[j];
	}
	long long maxi=E.dataLen-E.time2index(totalTime)-1;
	for (i=0;i<7/*maxi*/; i++) {
		//printf("index=%lld ", i);
		if (check(E, i)) {
			fprintf(output, "%f\n", E.index2time(i));
		}
	}
	fclose(output);
	return 0;
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
