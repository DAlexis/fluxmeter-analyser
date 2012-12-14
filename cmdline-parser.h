#ifndef _CMDLINEPARSER_H_
#define _CMDLINEPARSER_H_

#include <string>

using namespace std;

struct jobList 
{
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
	
	bool need_strikes_detection;
	bool need_strikes_stat;
	string output_stat_filename;
	
	string pattern;
	bool need_strikes_list;
	bool need_simple;
	bool need_pattern;
	string output_strikes_filename;
	string simple_config;
	
	bool need_trace;
	double trace_begin, trace_end;
	
	string fresh_file_format;

	bool need_renorm;
	float renorm_k;

	bool need_trunc_text_out;
	int text_out_trunc;
	
	
	jobList():  need_average(false), need_trunc(false), need_rc(false), need_nl(false),
				need_quantum_filtering(false), need_binary_input(false), need_fresh_input(false), 
				need_text_out(false), need_binary_out(false),   need_strikes_detection(false), need_strikes_stat(false),
				  need_strikes_list(false),  
				
				need_simple(false), need_pattern(false), need_trace(false),
				fresh_file_format("ipf"), need_renorm(false), renorm_k(1), need_trunc_text_out(false), text_out_trunc(10) {};
	
	int parse(int argc, char **argv);
	void printHelp();
};

#endif
