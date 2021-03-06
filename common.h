#ifndef oim_common_h
#define oim_common_h

#include <random>
//#include <sys/time.h>
#include <windows.h>
#include <time.h>
#include <memory>
#include "unistd.h"
#include <ios>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdint>
#include <Mmsystem.h>

#define THETA_OFFSET 5
#define MAX_R 10000000

extern double sampling_time;
extern double choosing_time;
extern double reused_ratio;

typedef uint32_t unode_int; // Type for node ids (can be changed into 32 or 64 bits)

typedef struct {
	unode_int source;
	unode_int target;
	unsigned int trial;
} TrialType;

/**
Builds a seed using nanoseconds to avoid same results.
*/
//int seed_ns() {
//	struct timespec ts;
//	clock_gettime(CLOCK_MONOTONIC, &ts);
//	return (int)ts.tv_nsec;
//}
int seed_ns()
{
	_LARGE_INTEGER time_start;
	QueryPerformanceCounter(&time_start);
	return abs((int)time_start.QuadPart);
}
//int seed_ns2()
//{
//	DWORD   t1, t2;
//	t1 = timeGetTime();
//	return t1;
//}


//typedef unode_int long timestamp_t;

/**
Returns number of microseconds since the Epoch.
*/
//static timestamp_t get_timestamp() {
//	struct timeval now;
//	gettimeofday(&now, NULL);
//	return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
//}

double sqr(double t) {
	return t * t;
}

//void process_mem_usage(double &vm_usage, double &resident_set) {
//	using std::ios_base;
//	using std::ifstream;
//	using std::string;
//
//	vm_usage = 0.0;
//	resident_set = 0.0;
//
//	// 'file' stat seems to give the most reliable results
//	ifstream stat_stream("/proc/self/stat", ios_base::in);
//
//	// dummy vars for leading entries in stat that we don't care about
//	string pid, comm, state, ppid, pgrp, session, tty_nr;
//	string tpgid, flags, minflt, cminflt, majflt, cmajflt;
//	string utime, stime, cutime, cstime, priority, nice;
//	string O, itrealvalue, starttime;
//
//	// the two fields we want
//	unode_int vsize;
//	long rss;
//
//	stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
//		>> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
//		>> utime >> stime >> cutime >> cstime >> priority >> nice
//		>> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest
//
//	stat_stream.close();
//
//	long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
//	vm_usage = vsize / 1024.0;
//	resident_set = rss * page_size_kb;
//}
//
//double disp_mem_usage() {
//	double vm, rss;
//	process_mem_usage(vm, rss);
//	vm /= 1024;
//	rss /= 1024;
//	return rss;
//}

#endif
