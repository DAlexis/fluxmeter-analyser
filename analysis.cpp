#include <stdio.h>
#include <string>
#include <math.h>
#include <string.h>

#include "analysis.h"

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
	printf("Pattern with length of %d successfuly readed... ", len);
	fclose(input);
	return 0;
}

int pattern::readSimplePattern(string filename)
{
	
	FILE *input=fopen(filename.c_str(), "r");
	if (!input) {
		printf("Can\'t open file %s for reading config!\n", filename.c_str());
		return -2;
	}
	
	char str[255];
	while (!feof(input))
	{
		fscanf(input, "%s", str);
		
		if (strcmp(str, "preTime")==0) {		
			fscanf(input, "%f", &preTime);
			continue;
		}
		if (strcmp(str, "pre_dfdt")==0) {		
			fscanf(input, "%f", &pre_dfdt);
			continue;
		}
		if (strcmp(str, "stepTime")==0) {		
			fscanf(input, "%f", &stepTime);
			continue;
		}
		if (strcmp(str, "stepValue")==0) {		
			fscanf(input, "%f", &stepValue);
			continue;
		}
		if (strcmp(str, "needReverse")==0) {		
			fscanf(input, "%d", &needReverse);
			continue;
		}
		if (strcmp(str, "reverseTime")==0) {		
			fscanf(input, "%f", &reverseTime);
			continue;
		}
		if (strcmp(str, "freezeTime")==0) {		
			fscanf(input, "%f", &freezeTime);
			continue;
		}
	}
	
	printf("Done.\n");
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

void pattern::resetFreeze()
{
	freezeEnable=false;
}

void pattern::countFreezeIndex(dataContainer& E)
{
	freezeDInd=E.time2index(freezeTime);
}

bool pattern::checkResConsFreeze(long long &ind, bool print)
{
	if (!freezeEnable) {
		freezeEnable=true;
		lastDetection=ind;
		return false;
	}
	if ((ind-lastDetection)>freezeDInd) {
		lastDetection=ind;
		return true;
	} else {
		if (print) printf ("  But freeze time. \n");
		lastDetection=ind;
		return false;
	}
}

bool pattern::simpleCheck(dataContainer& E, long long index, bool print)
{
	int di=E.time2index(dt);
	if (di==0) di=1;
	dt=E.index2time(di);
	
	long long i=index;
	long long maxi=E.time2index(preTime)+index;
	
	if (print) printf ("\nt=%f: ", E.index2time(index));
	
	//stage1
	for (;i<=maxi; i++)
	{
		if ( m_abs( (E.E[i+di]-E.E[i])/dt ) > pre_dfdt) return false;
	}
	
	
	if (print) printf ("  Stage 1 completed\n");
	
	//stage2
	float maxdiff=0, step_sign;
	float first=E.E[i];
	
	maxi += E.time2index(stepTime);
	for (;i<=maxi; i++)
		if ( m_abs(E.E[i]-first) > maxdiff){
			maxdiff=m_abs(E.E[i]-first);
			step_sign=m_sign(E.E[i]-first);
		}
		

	if (maxdiff<stepValue) return false;
	if (print) printf ("  Stage 2 completed\n");
	
	//stage3
	
	if (!needReverse) return true;
	
	maxi += E.time2index(reverseTime);
	for (;i<=maxi; i++)
		if (step_sign != m_sign((E.E[i+di]-E.E[i])/dt) ) return true;
	if (print) printf ("  Stage 3 completed\n");
	
	
	return false;
}

int pattern::outStrikes(dataContainer& E, strikesClass& outp, char method)
{
	return outStrikes(E, outp, method, -1, -1);
}

int pattern::outStrikes(dataContainer& E, strikesClass& outp, char method, float tr_beg, float tr_end)
{
	long long i;
	int j;
	float totalTime=0;
	for (j=0; j<len; j++) {
		totalTime+=time[j];
	}
	long long maxi=E.dataLen-E.time2index(totalTime)-len;
	long long i_trBeg=E.time2index(tr_beg), i_trEnd=E.time2index(tr_end);
	bool need_tr=false;
	
	bool result=false;
	
	countFreezeIndex(E);
	resetFreeze();
	
	for (i=0; i<maxi; i++) {
		if (i>i_trEnd)
			need_tr=false;
		else if (i>i_trBeg)
			need_tr=true;
		
		if (method==AM_PATTERN) result=check(E, i, need_tr);
			else if (method==AM_SIMPLE) result=simpleCheck(E, i, need_tr);
			else { printf("Strange method!\n"); return -3;}
		
		if (result) result=checkResConsFreeze(i, need_tr);
		
		if (result) {
			outp.add(E.index2time(i)+E.time_shift, E.E[i]);
		}
	}
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
		for (j=0; j<range && offset+j<E.dataLen; j++) {
			newE[i]+=E.E[offset+j];
		}
		newE[i]/=j;
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

#define MAX_STEP_INCREASE		9

float nlStep(float x, float par1, float par2)
{
	float res=_pow(x, par1)*par2;
	
	float xMax=x*MAX_STEP_INCREASE;
	if (m_abs(res)>m_abs(xMax))
		return xMax;
	return res;
}

void nlF1(dataContainer& E, float par1, float par2)
{
	long long i;
	float diff_t=E.index2time(1);
	for (i=1; i<E.dataLen; i++) {
		E.E[i]=E.E[i-1]+ nlStep( (E.E[i]-E.E[i-1]), par1, par2) *diff_t;
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

void renorm(dataContainer& E, float k)
{
	long long i;
	for (i=1; i<E.dataLen; i++) {
		E.E[i]*=k;
	}
}
