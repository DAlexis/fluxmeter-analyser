#include <stdio.h>
#include <string>
#include "analysis.h"
#include "base.h" 
#include <math.h>

using namespace std;

// Some simple math

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

// Class pattern

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


bool pattern::check(dataContainer& E, long long index, bool print)
{
	int di=E.time2index(dt);
	if (di==0) di=1;
	dt=E.index2time(di);
	
	long long i=index;
	float df_dt=(E.E[i+di]-E.E[i])/dt;
	int last_sign=1;
	int stage;
	int in_stage;
	int count_in_stage;


	if (print) printf ("\nt=%f Starting detection, df_dt=%f, dt=%f, di=%d\n", E.index2time(index), df_dt, dt, di);

	
	for (stage=0; stage<len; stage++) {

		count_in_stage=E.time2index(time[stage]); // time is RELATIVE previous mark!


		if (print) printf("stage=%d, time[stage]=%f, count_in_stage=%d\n",stage, time[stage],count_in_stage);

		for (in_stage=0; in_stage<count_in_stage; in_stage++, i++) {
			
			df_dt=(E.E[i+di]-E.E[i])/dt;
			if (print) printf ("  in_stage=%d, sign[stage]=%c, m_sign(df_dt)=%d, df_dt=%f\n", in_stage, sign[stage], m_sign(df_dt), df_dt);
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
		if (print) printf("stage %d done\n", stage);
	}
	return true;
}

int pattern::outStrikes(dataContainer& E, string& filename)
{
	return outStrikes(E, filename, -1, -1);
}

int pattern::outStrikes(dataContainer& E, string& filename, float tr_beg, float tr_end)
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
	long long maxi=E.dataLen-E.time2index(totalTime)-len;
	long long i_trBeg=E.time2index(tr_beg), i_trEnd=E.time2index(tr_end);
	bool need_tr=false;
	
	for (i=0; i<maxi; i++) {
		//printf("index=%lld ", i);
		if (i>i_trEnd)
			need_tr=false;
		else if (i>i_trBeg)
			need_tr=true;
			
		if (check(E, i, need_tr)) {
			fprintf(output, "%f %f\n", E.index2time(i), E.E[i]);
		}
	}
	fclose(output);
	return 0;
}

// Functions

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
	//printf ("TODO\n");
	long long i;
	float diff_t=E.index2time(1);
	for (i=1; i<E.dataLen; i++) {
		E.E[i]=E.E[i-1]+(E.E[i]-E.E[i-1])*diff_t/rc;
	}	
}

float _pow(float a, float b)
{
	if (a<0) return -pow(-a, b);
	return pow(a, b);
}  

void nlF1(dataContainer& E, float par1, float par2)
{
	long long i;
	float diff_t=E.index2time(1);
	for (i=1; i<E.dataLen; i++) {
		E.E[i]=E.E[i-1]+ _pow( (E.E[i]-E.E[i-1]), par1) *diff_t * par2;
	}	
}

void quantumFilter(dataContainer& E, float quantum)
{
	long long i;
	for (i=1; i<E.dataLen; i++) {
		if ( m_abs(E.E[i]-E.E[i-1]) <= quantum )
			E.E[i]=E.E[i-1];
	}
}
