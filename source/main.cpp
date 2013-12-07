// main.cpp
#include <stdio.h>

#include "system-class.h"

using namespace std; 

/////////////////////////////////////////////////////////////////// main
int main(int argc, char* argv[])
{
	int res=0;
	systemClass sys;
	
	res=sys.getCmdline(argc, argv);
	if (res) return res;
	
	res=sys.input();
	if (res) return res;
	
	sys.process();
	
	res=sys.configDetector();
	switch (res) {
		case 0:	sys.detect(); break;
		case 1: break;
		default: return res;
	}	
	
	res=sys.output(); 
	if (res) return res;
	
	return 0;
}
