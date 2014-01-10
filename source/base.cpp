#include "base.h"
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include <vector>
 
using namespace std;

#define TEXT_BUFFER_SIZE	100000


const double timeLen=24*3600;

#pragma pack(push, 1)

struct dataLine {
	char time[8];
	char comma1;
	char E[5];
	char comma2;
	char strangeNumber;
	char NL[2];
}; 

#pragma pack(pop)

dataContainer::dataContainer(): time_shift(0), E(NULL), text_out_step(1)
{
}

dataContainer::~dataContainer()
{
	if (E) delete[] E;
}

int dataContainer::binaryInput(string& filename)
{
	struct stat inpStat;
	
	if (stat(filename.c_str(), &inpStat)==-1) {
		printf("Can\'t get size for %s.\n", filename.c_str());
		return -2;
	}
	
	long long fileSize=inpStat.st_size;	
	
	FILE *input=fopen(filename.c_str(), "rb");
	
	if (!input) {
		printf("Can\'t open input file for reading\n");
		return -2;
	}
	
	long long newDataLen=fileSize/sizeof(float);
	E=new float[newDataLen];
	
	dataLen=fread(E, sizeof(float), newDataLen, input);
	
	printf("Successfully readed %lld of %lld lines\n", dataLen, newDataLen);
	fclose(input);
	return 0;
}

int dataContainer::binaryOutput(string& filename)
{
	FILE *output=fopen(filename.c_str(), "wb");
	if (!output) {
		printf("Can\'t open output file %s for writeng!\n", filename.c_str());
		return -2;
	}
	long long writen=fwrite(E, sizeof(float), dataLen, output);
	printf("Successfully writed %lld of %lld records\n", writen, dataLen);
	fclose(output);
	return 0;
}

int dataContainer::readEFM(string& fileName)
{
	struct stat inpStat;
	printf("Reading efm data from %s...\n", fileName.c_str());
	if (stat(fileName.c_str(), &inpStat)==-1) {
		printf("Can\'t get size for %s.\n", fileName.c_str());
		return -2;
	}
	
    long long fileSize=inpStat.st_size;
    printf ("Size is: %lld, ", fileSize);
    
    
    char* buffer = new char[fileSize];
    FILE *input=fopen(fileName.c_str(), "rb");
    long long readed=fread(buffer, sizeof(char), fileSize, input);
    fclose(input);
    printf ("readed bytes: %lld, ", readed);
    dataLen = 0;
    vector<double> newE;
    for (long long i=0; i<readed;)
    {
        // if some empty lines
        while (i<readed && (buffer[i] == 13 || buffer[i] == 10)) i++;
        if (i == readed) break;
        // Skipping time 
        while (i<readed && buffer[i] != ',') i++;
        if (i >= readed) break;
        dataLen++;
        // Skip comma
        i++;
        double sign = 1;
        if (buffer[i] == '-') sign = -1;
        double nextE=0;
        while (buffer[++i] != '.') {
            nextE = nextE*10 + buffer[i] - '0';
        }
        // Skip dot
        double mul = 0.1;
        while (buffer[++i] != ',') {
            nextE += mul*(buffer[i] - '0');
            mul /= 10.0;
        }
        nextE *= sign;
        newE.push_back(nextE);
        // Waiting for eol
        while (i<readed && buffer[i] != 13 && buffer[i] != 10) i++;
        if (i == readed) break;
    }
    printf("readed records: %lld.\n", dataLen);
    
    E=new float[dataLen];
    for (int i=0; i<dataLen; i++)
    {
        E[i] = newE[i];
    }
    
    return 0;
}


int dataContainer::readDAT(string& fileName, int ncols, int col)
{
	struct stat inpStat;
	printf("Reading fresh data from %s...\n", fileName.c_str());
	if (stat(fileName.c_str(), &inpStat)==-1) {
		printf("Can\'t get size for %s.\n", fileName.c_str());
		return -2;
	}
	long long fileSize=inpStat.st_size;
	
	FILE *input=fopen(fileName.c_str(), "rb");
	if (!input) {
		printf("Can\'t open input file for reading\n");
		return -2;
	}	
	
	char *buffer=new char[fileSize];
	long long readed=fread(buffer, sizeof(char), fileSize, input);
	printf("Successfully readed %lld of %lld bytes\n", readed, fileSize);
	if (readed!=fileSize) printf("!!! Hm. Not all file was readed.\n");
	E=new float [MYZA_STR_COUNT];
	long long i, j=0;
	bool minus=false;
	
	int k;
	char stopchar; 
	if (col==ncols) stopchar=0x0D; else stopchar='\t';
	
	for (i=0; i<MYZA_STR_COUNT; i++) {
					
		E[i]=0;
		// Skipping columns before col
		for (k=1; k<col; k++) {
			while (buffer[j]!='\t') j++;
			j++;
		}
		// Reading column we interested in	
		if (buffer[j]=='-') {
			minus=true;
			j++;
		}
			else minus=false;
	
		while (buffer[j]!=stopchar) {
			E[i]=E[i]*10+(buffer[j]-'0');
			j++;
		}
		  
		// Waiting for eol
		while (buffer[j]!=0x0A) j++;
		j++;
		if (minus) E[i]=-E[i];
		
		if (j>=fileSize) {
			break;
		}
	}
	dataLen=i;
	delete[] buffer;
	
	return 0;
}

int dataContainer::readFresh(string& fileName, string& inpFmt)
{
	if (inpFmt=="dat") {
		return readDAT(fileName, 2,1); /// @todo [Hi] number of columns and column to read from should be configurable!
	} else if (inpFmt=="efm") {
		// Reading data in 00:00:00,+0.09,0-like format
		return readEFM(fileName);
	}
	printf("Unknown file format \"%s\"", inpFmt.c_str());
	return -2;
} 

long long dataContainer::time2index(double time)
{
	return (long long) (( (float) (time-time_shift) * dataLen )/timeLen);
}

float dataContainer::index2time(long long ind)
{
	return ((float) ind * timeLen)/dataLen+time_shift;
}

long long dataContainer::timeInterval2index(double time)
{
	return (long long) (((float)time * dataLen)/timeLen);
}

float dataContainer::index2timeInterval(long long ind)
{
	return ((float) ind * timeLen)/dataLen;
}

#define LINE_LEN_MAX	100

int dataContainer::textOutput(string& filename)
{
	FILE *output=fopen(filename.c_str(), "wb");
	if (!output) {
		printf("Can\'t open output file %s for writeng!\n", filename.c_str());
		return -2;
	}
	
	char buffer[LINE_LEN_MAX];
	long long i;
	int j=0, k=0;
	char bigBuffer[TEXT_BUFFER_SIZE]="";
	
	long long iLeft=0,  iRight=dataLen;
	/*
	if (use_limits) {
		iLeft=time2index(left_limit);
		iRight=time2index(right_limit);
	}*/
	
	for (i=iLeft; i<iRight; i+=text_out_step) {
		
		if (needContinue(index2time(i))) continue;
		
		sprintf(buffer, "%f %f\n", index2time(i), E[i]);
		
		for(k=0; buffer[k]!='\0'; k++) {
			bigBuffer[j++]=buffer[k];
		}
		
		if (j > TEXT_BUFFER_SIZE-LINE_LEN_MAX) {
			fwrite(bigBuffer, 1, j, output);
			j=0;
		}
	}
	
	if (j != 0) {
		fwrite(bigBuffer, 1, j, output);
	}
	
	fclose(output);
	printf("Done.\n");
	return 0;
}

void dataContainer::setTimeShift(float shift)
{
	time_shift=shift*60*60;
}
