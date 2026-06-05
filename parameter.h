#ifndef PARAMETERS_H_
#define PARAMETERS_H_
#include <sstream>
#include <algorithm>
#include "utils.h"
using std::min;
using std::max;
using std::swap;
using std::string;

constexpr double min_q_val = 0.001;
constexpr double max_q_val = 0.1;
constexpr double q_val_interv = max_q_val - min_q_val;
constexpr uint min_bw = 150U;
constexpr uint max_bw = 450U;
constexpr uint bw_interv = max_bw - min_bw;
constexpr uint min_min_fold = 2U;
constexpr uint max_min_fold = 10U;
constexpr uint min_fold_interv = max_min_fold - min_min_fold;
constexpr uint min_max_fold = 15U;
constexpr uint max_max_fold = 50U;
constexpr uint max_fold_interv = max_max_fold - min_max_fold;
constexpr uint min_ext_size = 100U;
constexpr uint max_ext_size = 300U;
constexpr uint ext_size_interv = max_ext_size - min_ext_size;

struct parameter {
	uint bw;
	uint min_fold, max_fold;
	uint ext_size;
	double q_val;
	double fitness;
	bool same;
	void crossover(parameter &other);
	void mutate(void);
	string get_exec_str(const char *input_fp, const char *macs_dir, const char *other_params);
	double get_fitness_from_file(const char *fp);
	parameter() {
		q_val = get_random(min_q_val, max_q_val);
		bw = get_random(min_bw, max_bw);
		min_fold = get_random(min_min_fold, max_min_fold); 
		max_fold = get_random(min_max_fold, max_max_fold); 
		ext_size = get_random(min_ext_size, max_ext_size);
		fitness = 0.0;
		same = false;
	};
	bool operator==(const parameter &other) {
		return bw == other.bw && min_fold == other.min_fold && max_fold == other.max_fold && ext_size == other.ext_size && q_val == other.q_val;
	};
};

#endif
