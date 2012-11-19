#include "base.h"
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <string>

using namespace std;

#define TEXT_BUFFER_SIZE	10000

const double timeLen=24*3600;

struct dataLine {
	char time[8];
	char comma1;
	char E[5];
	char comma2;
	char strangeNumber;
	char NL[2];
};

dataContainer::~dataContainer()
{
	delete[] E;
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

int dataContainer::readFresh(string& fileName)
{
	struct stat inpStat;
	
	printf("Reading fresh data from %s...\n", fileName.c_str());
	if (stat(fileName.c_str(), &inpStat)==-1) {
		printf("Can\'t get size for %s.\n", fileName.c_str());
		return -2;
	}
	
	long long fileSize=inpStat.st_size, linesCount=(long long) fileSize/sizeof(dataLine);
	printf ("Size is: %lld, lines: %lld\n", fileSize, linesCount);

	dataLine* data=new dataLine[(long long) fileSize/sizeof(dataLine)+10];
	FILE *input=fopen(fileName.c_str(), "rb");
	if (!input) {
		printf("Can\'t open input file for reading\n");
		return -2;
	}	
	long long readed=fread(data, sizeof(dataLine), linesCount, input);
	dataLen=readed;
	printf("Successfully readed %lld of %lld lines\n", readed, linesCount);
	fclose(input);
	if (dataLen==0) {
		printf("No data was readed!\n");
		return -2;
	}
	
	E=new float[readed];
	long long i;
	for (i=0; i<dataLen; i++) {
		E[i]=(data[i].E[1]-'0')+(data[i].E[3]-'0')/10.0+(data[i].E[4]-'0')/100.0;
		if (data[i].E[0]=='-') E[i]=-E[i];
	}
	delete[] data;
	//printf("%f %f\n", index2time(0), E[0]);
	return 0;
}

long long dataContainer::time2index(double time)
{
	return (long long) (((float)time * dataLen)/timeLen);
}

float dataContainer::index2time(long long ind)
{
	return ((float) ind * timeLen)/dataLen;
}

int dataContainer::textOutput(string& filename)
{
	FILE *output=fopen(filename.c_str(), "wb");
	if (!output) {
		printf("Can\'t open output file %s for writeng!\n", filename.c_str());
		return -2;
	}
	
	char buffer[100];
	long long i;
	int j=0, k=0;
	char bigBuffer[TEXT_BUFFER_SIZE]="";
	for (i=0; i<dataLen; i++) {
		sprintf(buffer, "%f %f\n", index2time(i), E[i]);
		k=0;
		while (buffer[k]!='\0') {
			bigBuffer[j++]=buffer[k++];
		}
		if (j>TEXT_BUFFER_SIZE-100) {
			fwrite(bigBuffer, 1, j, output);
			j=0;
			//break;
		}
	}
	fclose(output);
	return 0;
}
