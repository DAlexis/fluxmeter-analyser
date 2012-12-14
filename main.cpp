// main.cpp

#include <stdio.h>
#include <string>
#include <vector>


#include <sys/types.h>
#include <sys/stat.h>

//#include "inpout.h"

#include "base.h"
#include "analysis.h"
#include "statistics.h"
#include "cmdline-parser.h"

using namespace std; 

/////////////////////////////////////////////////////////////////// main
int main(int argc, char* argv[])
{
	jobList job;
	int res=0;
	res=job.parse(argc, argv);
	if (res) return res;
	
	dataContainer data;
	
	// Reading data
	
	if (job.need_fresh_input) {
		res=data.readFresh(job.fresh_input_filename, job.fresh_file_format);
	} else if (job.need_binary_input) {
		res=data.binaryInput(job.binary_filename);
	}
	
	if (res) return res;
	if (!job.need_fresh_input && !job.need_binary_input) {
		printf("Hm. I can\'t do anything without input file.\n");
		return -3;
	}
	
	// Conversions
	if (job.need_renorm) {
		printf("Renorming data with k=%f... ", job.renorm_k);
		renorm(data, job.renorm_k);
		printf("Done\n");
	}
	
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
	pattern patt;
	strikesClass strikes;
	
	// Detection
	if (job.need_pattern) {
		printf("Reading pattern from %s... ", job.pattern.c_str());
		res=patt.readPattern(job.pattern);
		if (res) return res;
		
		if (job.need_trace)
			res=patt.outStrikes(data, strikes, AM_PATTERN, job.trace_begin, job.trace_end);
		else
			res=patt.outStrikes(data, strikes, AM_PATTERN);
		printf ("Done.\n");
	} else if (job.need_simple) {
		printf("Reading config from %s... ", job.simple_config.c_str());
		res=patt.readSimplePattern(job.simple_config);
		if (res) return res;
		
		if (job.need_trace)
			res=patt.outStrikes(data, strikes, AM_SIMPLE, job.trace_begin, job.trace_end);
		else
			res=patt.outStrikes(data, strikes, AM_SIMPLE);
		printf ("Done.\n");		
	} else printf("No detection option enabed.\n");
	
	if (job.need_strikes_list) {
		printf("Writing strikes list to %s... ", job.output_strikes_filename.c_str());
		res=strikes.print(job.output_strikes_filename);
		if (res) return res;
	}
	
	if (job.need_strikes_stat) {
		printf("Writing strikes hist to %s... ", job.output_stat_filename.c_str());
		res=strikes.printHist(job.output_stat_filename);
		if (res) return res;
	}
	
	if (job.need_binary_out) {
		printf("Writing binary data to %s... ", job.output_binary_filename.c_str());
		res=data.binaryOutput(job.output_binary_filename);
		if (res) return res;
	}
	if (job.need_text_out) {
		printf("Writing text data to %s... ", job.output_text_filename.c_str());
		res=data.textOutput(job.output_text_filename);
		if (res) return res;
	}
	return 0;
}
