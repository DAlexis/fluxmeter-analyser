// main.cpp

#include <stdio.h>
#include <string>
#include <vector>


#include <sys/types.h>
#include <sys/stat.h>

//#include "inpout.h"

#include "system-class.h"

using namespace std; 

/////////////////////////////////////////////////////////////////// main
int main(int argc, char* argv[])
{
	systemClass sys;	
	int res=0;
	
	res=sys.getCmdline(argc, argv);
	if (res) return res;
	
	res=sys.input();
	if (res) return res;
	
	sys.process();
	
	res=sys.configDetector();
	if (res) return res;
	
	sys.detect();
	
	res=sys.output();
	if (res) return res;
	
	return 0;
}
