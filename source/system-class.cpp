#include "system-class.h"

#include <stdio.h>

int systemClass::getCmdline(int argc, char** argv)
{
	int res=0;
	res=job.parse(argc, argv);
	data.text_out_step=job.text_out_trunc;
	return res;	
}

int systemClass::input()
{
	int res=0;
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
	return 0;
}

void systemClass::process()
{
	if (job.time_shift!=0) {
		printf ("Using time shift %f.\n", job.time_shift);
		data.setTimeShift(job.time_shift);
		strikes.time_shift=job.time_shift;
	}
	if (job.use_limits) {
		printf("Using limits %f H --- %f H.\n", job.left_limit, job.right_limit);
		/*strikes.setLimits( job.left_limit*3600-job.time_shift, job.right_limit*3600-job.time_shift );
		data.setLimits( job.left_limit*3600-job.time_shift, job.right_limit*3600-job.time_shift );*/
		strikes.setLimits( job.left_limit*3600, job.right_limit*3600);
		data.setLimits( job.left_limit*3600, job.right_limit*3600 );
	}
	
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
}

int systemClass::configDetector()
{
	int res;
	if (job.need_pattern) {
		printf("Reading pattern from %s... ", job.pattern.c_str());
		res=patt.readPattern(job.pattern);
		if (res) {
			job.need_pattern=false;
			return res;
		}
		printf ("Done.\n");
	} else if (job.need_simple) {
		printf("Reading config from %s... ", job.simple_config.c_str());
		res=patt.readSimplePattern(job.simple_config);
		if (res) {
			job.need_simple=false;
			return res;
		}
		printf ("Done.\n");		
	} else {
		printf("No detection option enabed.\n");
		return 1;
	}
	return 0;
}

void systemClass::detect()
{
	if (job.need_pattern) {
		printf("Detecting using patter file... ");
		
		if (job.need_trace)
			patt.outStrikes(data, strikes, AM_PATTERN, job.trace_begin, job.trace_end);
		else
			patt.outStrikes(data, strikes, AM_PATTERN);
			
		printf ("Done.\n");
	} else if (job.need_simple) {
		printf("Detecting using config... ");
				
		if (job.need_trace)
			patt.outStrikes(data, strikes, AM_SIMPLE, job.trace_begin, job.trace_end);
		else
			patt.outStrikes(data, strikes, AM_SIMPLE);
			
		printf ("Done.\n");		
	} //else printf("No detection option enabed.\n");
}

int systemClass::output()
{
	int res=0;
	if (job.need_strikes_list) {
		printf("Writing strikes list to %s... ", job.output_strikes_filename.c_str());
		res=strikes.print(job.output_strikes_filename);
		if (res) return res;
	}
	
	if (job.need_strikes_stat) {
		printf("Writing strikes per minute histoghram to %s... ", job.output_stat_filename.c_str());
		res=strikes.printHist(job.output_stat_filename);
		if (res) return res;
	}
	
	if (job.need_strike_intervals_stat) {
		printf("Writing strike interval histoghram to %s... ", job.output_interval_stat_filename.c_str());
		res=strikes.printIntervalsHistogram(job.output_interval_stat_filename, 10, true, 120);
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
	
	if (job.need_report) {
		res=strikes.addReport(job.report_filename);
		if (res) return res;
	}
	
	return 0;
}
