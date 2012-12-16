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
	sys.getCmdline(argc, argv);
	sys.input();
	sys.process();
	sys.configDetector();
	sys.detect();
	sys.output();
	
	return 0;
}
