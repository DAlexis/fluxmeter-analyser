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
	
	float time_shift;
	
	jobList();
	~jobList();
	
	int parse(int argc, char **argv);
	void printHelp();
};

#endif
