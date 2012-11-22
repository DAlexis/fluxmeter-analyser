// main.cpp

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

//#include "inpout.h"

#include "base.h"
#include "analysis.h"

using namespace std; 

struct jobList {
	bool need_average;
	int average_range;
	bool need_trunc;
	int trunc_range;
	bool need_rc;
	bool need_nl;
	float par1, par2;
	
	float rc;
	bool need_quantum_filtering;
	float quantum_size;
	
	
	bool need_binary_input;
	bool need_fresh_input;
	string binary_filename;
	string fresh_input_filename;
	
	bool need_text_out;
	bool need_binary_out;
	string output_binary_filename;
	string output_text_filename;
	
	bool need_strikes;
	string pattern;
	string output_strikes;
	
	bool need_trace;
	double trace_begin, trace_end;
	
	jobList(): need_binary_input(0), need_fresh_input(0), 
				need_binary_out(0), need_text_out(0), need_average(0), 
				need_trunc(0), need_rc(0), need_strikes(0), need_trace(0), need_quantum_filtering(0), need_nl(0) {};
};
/*
jobList::jobList()
{
	binary_filename="";
	pattern="";
	output_binary_filename="";
	output_text_filename="";
	output_strikes="";
}*/

void printHelp()
{
	printf("\nflux-analysis <options> filename\n");
	printf("\nOptions:\n");
	printf("  --help, -h                       - print this help\n");
	printf(" \nInput:\n");
	printf("  --input-fresh, -f <filename>     - input from file with mesured data\n");
	printf("  --input-binary, -i <filename>    - input from binary file\n");
	printf(" \nOutput:\n");
	printf("  --output-binary, -o <filename>   - file to binary out\n");
	printf("  --output-text, -t <filename>     - file to text out\n");
	printf("  --strikes-list, -s <filename>    - put strikes list to file\n");
	printf(" \nOperating:\n");
	printf("  --pattern, -p <filename>         - select pattern for strike detection\n");
	printf("  --average, -v <range>            - keep i number average (i-range, i+range)\n");
	printf("  --trunc, -t <range>              - keep data shorter counting average of every <range>\n");
	printf("  --rc, -r <time>                  - use RC-filter for signal with RC=time\n");
	printf("  --quantum-filtering, -q <quantum>- pass data through the trigger\n");
}


/////////////////////////////////////////////////////////////////// main
int main(int argc, char* argv[])
{
	if (argc==1) {
		printf("Arguments not specified!\n\n");
		printHelp();
		return -1;
	}	
	jobList job;
	
	int argNum=1;
	string outputFile="", inputFile="";
	while (argNum!=argc && argv[argNum][0]=='-') {
		if (strcmp(argv[argNum], "-h")==0 || strcmp(argv[argNum], "--help")==0) {
			printHelp();
			return 0;
		}
		if (strcmp(argv[argNum], "--input-fresh")==0 || strcmp(argv[argNum], "-f")==0) {
			if (argc == ++argNum) {
				printf("Expected: data file.\n");
				return -1;
			}
			job.need_fresh_input=1;
			job.fresh_input_filename=argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--quantum-filtering")==0 || strcmp(argv[argNum], "-q")==0) {
			if (argc == ++argNum) {
				printf("Expected: quantum size.\n");
				return -1;
			}
			job.need_quantum_filtering=1;
			job.quantum_size=atof(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--need-trace")==0 || strcmp(argv[argNum], "-T")==0) {
			job.need_trace=1;
			if (argc == ++argNum) {
				printf("Expected: trace begin.\n");
				return -1;
			}			
			job.trace_begin=atof(argv[argNum]);
			if (argc == ++argNum) {
				printf("Expected: trace end.\n");
				return -1;
			}			
			job.trace_end=atof(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--need-NL")==0 || strcmp(argv[argNum], "-n")==0) {
			job.need_nl=1;
			if (argc == ++argNum) {
				printf("Expected: exp.\n");
				return -1;
			}			
			job.par1=atof(argv[argNum]);
			if (argc == ++argNum) {
				printf("Expected: coefficient.\n");
				return -1;
			}			
			job.par2=atof(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--output-binary")==0 || strcmp(argv[argNum], "-o")==0) {
			if (argc == ++argNum) {
				printf("Expected: binary output file and input file.\n");
				return -1;
			}
			job.need_binary_out=1;
			job.output_binary_filename=argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--output-text")==0 || strcmp(argv[argNum], "-t")==0) {
			if (argc == ++argNum) {
				printf("Expected: text output file and input file.\n");
				return -1;
			}
			job.need_text_out=1;
			job.output_text_filename=argv[argNum];
			argNum++;
			//printf("dot\n");
			continue;
		}
		if (strcmp(argv[argNum], "--strikes-list")==0 || strcmp(argv[argNum], "-s")==0) {
			if (argc == ++argNum) {
				printf("Expected: pattern file expected.\n");
				return -1;
			}
			job.need_strikes=1;
			job.output_strikes=argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--average")==0 || strcmp(argv[argNum], "-a")==0) {
			if (argc == ++argNum) {
				printf("Expected: average range.\n");
				return -1;
			}
			job.need_average=1;
			job.average_range=atof(argv[argNum]);
			argNum++;
			continue;
		}		
		if (strcmp(argv[argNum], "--trunc")==0 || strcmp(argv[argNum], "-t")==0) {
			if (argc == ++argNum) {
				printf("Expected: trunc range.\n");
				return -1;
			}
			job.need_trunc=1;
			job.trunc_range=atoi(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--rc")==0 || strcmp(argv[argNum], "-r")==0) {
			if (argc == ++argNum) {
				printf("Expected: circuit time.\n");
				return -1;
			}
			job.need_rc=1;
			job.rc=atof(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--pattern")==0 || strcmp(argv[argNum], "-p")==0) {
			if (argc == ++argNum) {
				printf("Expected: pattern file.\n");
				return -1;
			}
			job.pattern=argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--input-binary")==0 || strcmp(argv[argNum], "-i")==0) {
			if (argc == ++argNum) {
				printf("Expected: input file.\n");
				return -1;
			}
			job.need_binary_input=1;
			job.binary_filename=argv[argNum];
			argNum++;
			continue;
		}
		printf("Unknown key: %s\n", argv[argNum]);
		return -1;
	}
	
	dataContainer data;
	
	// Reading data
	int res=0;
	if (job.need_fresh_input) {
		res=data.readFresh(job.fresh_input_filename);
	} else if (job.need_binary_input) {
		res=data.binaryInput(job.binary_filename);
	}
	if (res) return res;
	if (!job.need_fresh_input && !job.need_binary_input) {
		printf("Hm. I can\'t do anything without input file.\n");
		return -3;
	}
	pattern patt;
	// Conversions
	if (job.need_trunc) {
		printf("Truncating data with range %d mesurings... ", job.trunc_range);
		truncData(data, job.trunc_range);
		printf("Done\n");
	}
	if (job.need_average) {
		printf("Counting average of %d mesurings... ", job.average_range);
		averageData(data, job.average_range);
		printf("Done\n");
	}
	if (job.need_quantum_filtering) {
		printf("Applying differential Schmitt trigger with min. step %f ... ", job.quantum_size);
		quantumFilter(data, job.quantum_size);
		printf("Done\n");
	}
	if (job.need_rc) {
		printf("Applying rc-filter with RC=%f ... ", job.rc);
		rcData(data, job.rc);
		printf("Done\n");
	}
	if (job.need_nl) {
		printf("Applying NL-filter with exp=%f, A=%f... ", job.par1, job.par2);
		nlF1(data, job.par1, job.par2);
		printf("Done\n");
	}
	// Outouting data
	if (job.need_strikes) {
		if (job.pattern=="") {
			printf("No pattern for detection\n");
			return -2;
		}
		printf("Reading pattern from %s... ", job.pattern.c_str());
		res=patt.readPattern(job.pattern);
		if (res) return res;
		
		if (job.need_trace)
			res=patt.outStrikes(data, job.output_strikes, job.trace_begin, job.trace_end);
		else
			res=patt.outStrikes(data, job.output_strikes);
			
		if (res) return res;
		printf("Done\n");
	}	
	if (job.need_binary_out) {
		printf("Writing binary data to %s\n", job.output_binary_filename.c_str());
		res=data.binaryOutput(job.output_binary_filename);
		if (res) return res;
	}
	if (job.need_text_out) {
		printf("Writing text data to %s\n", job.output_text_filename.c_str());
		res=data.textOutput(job.output_text_filename);
		if (res) return res;
	}
	return 0;
}
