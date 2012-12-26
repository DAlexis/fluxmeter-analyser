#include "cmdline-parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

jobList::jobList():  need_average(false), need_trunc(false), need_rc(false), need_nl(false),
				need_quantum_filtering(false), need_binary_input(false), need_fresh_input(false), 
				need_text_out(false), need_binary_out(false),   need_strikes_detection(false),
				need_strikes_stat(false), need_strikes_list(false),  
				
				need_simple(false), need_pattern(false), need_trace(false),
				fresh_file_format("ipf"), need_renorm(false), renorm_k(1), 
				need_trunc_text_out(false), text_out_trunc(1), time_shift(0),
				need_report(false), use_limits(false), left_limit(0), right_limit(86400)
{
	//printf("JobList constructor\n");
};

jobList::~jobList()
{
	//printf("JobList destructor\n");
};


void jobList::printHelp()
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
	printf("  --strikes-stat, -M <filename>    - print strikes per minute\n");
	printf(" \nOperating:\n");
	printf("  --renorm, -r <coefficient>       - multiply every value by coefficient\n");
	printf("  --pattern, -p <filename>         - select pattern for strike detection\n");
	printf("  --average, -v <range>            - keep i number average (i-range, i+range)\n");
	printf("  --trunc, -u <range>              - keep data shorter counting average of every <range>\n");
	printf("  --rc, -r <time>                  - use RC-filter for signal with RC=time\n");
	printf("  --NL, -n <exp> <const>           - use non-linear differential filter for signal with parameters\n");
	printf("  --quantum-filtering, -q <quantum>- pass data through the trigger\n");
	printf("  --simple-detection, -S <config>  - simple detection algorythm\n");
	printf("  --time-shift, -z <shift>         - use time shift\n");
	printf(" \nOptions:\n");
	printf("  --trunc-text, -K <value>         - out only every <value> value when text outputing\n");
	printf("  --need-trace, -T <from> <to>     - pattern debug output\n");
	printf("  --fresh-format, -F <format>      - set \"fresh file\" format. Default is \'ipf\'\n");
	printf("	Now avaliable formats:\n");
	printf("    ipf, myza\n");	
	printf("  --report, -R, <filename>         - add count of strikes to <filename>\n");
	printf("  --limits, -l, <left> <right>     - output only in limits\n");
}

/*
bool parseTextParameter(char* longName, char* shortName, char* noValMsg, bool& flag, string& parameter, char** argv, int argc, int& testNum)
{
	if (strcmp(argv[testNum], longName)==0 || strcmp(argv[testNum], shortName)==0) {
		if (argc == ++testNum) {
			printf(noValMsg);
			return -1;
		}
		flag=true;
		parameter=argv[testNum];
		testNum++;
		return true;
	}
	return false;
}*/


int jobList::parse(int argc, char **argv)
{
	if (argc==1) {
		printf("Arguments not specified!\n\n");
		printHelp();
		return -1;
	}	
	
	int argNum=1;
	string outputFile="", inputFile="";
	
	while (argNum!=argc && argv[argNum][0]=='-') {
		if (strcmp(argv[argNum], "-h")==0 || strcmp(argv[argNum], "--help")==0) {
			printHelp();
			return 0;
		}
		if (strcmp(argv[argNum], "--report")==0 || strcmp(argv[argNum], "-R")==0) {
			if (argc == ++argNum) {
				printf("Expected: trucation value.\n");
				return -1;
			}
			need_report=true;
			report_filename =argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--trunc-text")==0 || strcmp(argv[argNum], "-K")==0) {
			if (argc == ++argNum) {
				printf("Expected: trucation value.\n");
				return -1;
			}
			need_trunc_text_out=true;
			text_out_trunc=atoi(argv[argNum]);
			argNum++;
			continue;
		}
		/*
		if ( parseTextParameter("--input-fresh", "-f",
		                       "Expected: data file.\n",
		                       need_fresh_input, fresh_input_filename,
		                       argv, argc, argNum) )
			continue;
			*/
		if (strcmp(argv[argNum], "--input-fresh")==0 || strcmp(argv[argNum], "-f")==0) {
			if (argc == ++argNum) {
				printf("Expected: data file.\n");
				return -1;
			}
			need_fresh_input=1;
			fresh_input_filename=argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--renorm")==0 || strcmp(argv[argNum], "-r")==0) {
			if (argc == ++argNum) {
				printf("Expected: coefficient.\n");
				return -1;
			}
			need_renorm=1;
			renorm_k=atof(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--quantum-filtering")==0 || strcmp(argv[argNum], "-q")==0) {
			if (argc == ++argNum) {
				printf("Expected: quantum size.\n");
				return -1;
			}
			need_quantum_filtering=1;
			quantum_size=atof(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--fresh-format")==0 || strcmp(argv[argNum], "-F")==0) {
			if (argc == ++argNum) {
				printf("Expected: input file format.\n");
				return -1;
			}
			fresh_file_format=argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--time-shift")==0 || strcmp(argv[argNum], "-z")==0) {
			if (argc == ++argNum) {
				printf("Expected: time shift.\n");
				return -1;
			}
			time_shift=atof(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--need-trace")==0 || strcmp(argv[argNum], "-T")==0) {
			need_trace=1;
			if (argc == ++argNum) {
				printf("Expected: trace begin.\n");
				return -1;
			}			
			trace_begin=atof(argv[argNum]);
			if (argc == ++argNum) {
				printf("Expected: trace end.\n");
				return -1;
			}			
			trace_end=atof(argv[argNum]);
			argNum++;
			continue; 
		}
		if (strcmp(argv[argNum], "--NL")==0 || strcmp(argv[argNum], "-n")==0) {
			need_nl=1;
			if (argc == ++argNum) {
				printf("Expected: exp.\n");
				return -1;
			}			
			par1=atof(argv[argNum]);
			if (argc == ++argNum) {
				printf("Expected: coefficient.\n");
				return -1;
			}			
			par2=atof(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--limits")==0 || strcmp(argv[argNum], "-l")==0) {
			use_limits=true;
			if (argc == ++argNum) {
				printf("Expected: left limit.\n");
				return -1;
			}			
			left_limit=atof(argv[argNum]);
			if (argc == ++argNum) {
				printf("Expected: right limit.\n");
				return -1;
			}			
			right_limit=atof(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--output-binary")==0 || strcmp(argv[argNum], "-o")==0) {
			if (argc == ++argNum) {
				printf("Expected: binary output file and input file.\n");
				return -1;
			}
			need_binary_out=1;
			output_binary_filename=argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--output-text")==0 || strcmp(argv[argNum], "-t")==0) {
			if (argc == ++argNum) {
				printf("Expected: text output file and input file.\n");
				return -1;
			}
			need_text_out=1;
			output_text_filename=argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--strikes-list")==0 || strcmp(argv[argNum], "-s")==0) {
			if (argc == ++argNum) {
				printf("Expected: filename.\n");
				return -1;
			}
			need_strikes_list=1;
			output_strikes_filename=argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--strikes-stat")==0 || strcmp(argv[argNum], "-M")==0) {
			if (argc == ++argNum) {
				printf("Expected: filename.\n");
				return -1;
			}
			need_strikes_stat=1;
			output_stat_filename=argv[argNum];
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--average")==0 || strcmp(argv[argNum], "-a")==0) {
			if (argc == ++argNum) {
				printf("Expected: average range.\n");
				return -1;
			}
			need_average=1;
			average_range=atof(argv[argNum]);
			argNum++;
			continue;
		}		
		if (strcmp(argv[argNum], "--trunc")==0 || strcmp(argv[argNum], "-u")==0) {
			if (argc == ++argNum) {
				printf("Expected: trunc range.\n");
				return -1;
			}
			need_trunc=1;
			trunc_range=atoi(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--rc")==0 || strcmp(argv[argNum], "-C")==0) {
			if (argc == ++argNum) {
				printf("Expected: circuit time.\n");
				return -1;
			}
			need_rc=1;
			rc=atof(argv[argNum]);
			argNum++;
			continue;
		}
		if (strcmp(argv[argNum], "--pattern")==0 || strcmp(argv[argNum], "-p")==0) {
			need_pattern=1;
			if (argc == ++argNum) {
				printf("Expected: pattern file.\n");
				return -1;
			}
			pattern=argv[argNum];
			argNum++;
			continue;
		}
		
		if (strcmp(argv[argNum], "--simple-detection")==0 || strcmp(argv[argNum], "-S")==0) {
			need_simple=1;
			if (argc == ++argNum) {
				printf("Expected: config file.\n");
				return -1;
			}
			simple_config=argv[argNum];
			argNum++;
			continue; 
		}
		
		if (strcmp(argv[argNum], "--input-binary")==0 || strcmp(argv[argNum], "-i")==0) {
			if (argc == ++argNum) {
				printf("Expected: input file.\n");
				return -1;
			}
			need_binary_input=1;
			binary_filename=argv[argNum];
			argNum++;
			continue;
		}
		printf("Unknown key: %s\n", argv[argNum]);
		return -1;
	}
	return 0;
}
