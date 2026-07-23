#ifndef PARAMETERS_H_
#define PARAMETERS_H_
#include <sstream>
#include <algorithm>
#include "utils.h"
using std::min;
using std::max;
using std::swap;

constexpr double min_q_val = 0.001;
constexpr double max_q_val = 0.1;
constexpr uint min_bw = 150U;
constexpr uint max_bw = 450U;
constexpr uint min_min_fold = 2U;
constexpr uint max_min_fold = 10U;
constexpr uint min_max_fold = 15U;
constexpr uint max_max_fold = 50U;
constexpr uint min_ext_size = 100U;
constexpr uint max_ext_size = 300U;

struct parameter {
	uint bw;
	uint min_fold, max_fold;
	uint ext_size;
	double q_val;
	double fitness;
	bool same;
	void crossover(parameter &other);
	void mutate(void);
	string get_exec_str(const string &input_fp, const string &macs_dir, const string &other_params);
	double get_fitness_from_file(const string &fp);
	parameter() {
		q_val = get_random(min_q_val, max_q_val);
		bw = get_random(min_bw, max_bw);
		min_fold = get_random(min_min_fold, max_min_fold); 
		max_fold = get_random(min_max_fold, max_max_fold); 
		ext_size = get_random(min_ext_size, max_ext_size);
		fitness = 0.0;
		same = false;
	};
	inline double execute_param(const string &input_fp, const string &macs_dir, const string &sh_cmd, const string &res_fp, const string &other_params) {
		same = true;
		string cmd = get_exec_str(input_fp, macs_dir, other_params);
		const auto cmd_c = cmd.c_str();
		if (std::system(cmd_c)) return 0.0;
		const auto sh_cmd_c = sh_cmd.c_str();
		if (std::system(sh_cmd_c)) std::exit(1);
		return get_fitness_from_file(res_fp);
	}
	bool operator==(const parameter &other) {
		return bw == other.bw && min_fold == other.min_fold && max_fold == other.max_fold && ext_size == other.ext_size && q_val == other.q_val;
	};
};

inline std::ostream& operator<<(std::ostream& os, parameter &param) {
	return os
		<< param.q_val
		<< ';' << param.bw
		<< ';' << param.min_fold
		<< ';' << param.max_fold
		<< ';' << param.ext_size
		<< ';' << param.fitness 
		<< ';' << param.same;
}

inline std::istream& operator>>(std::istream& is, parameter &param) {
	char delim;
	is  >> param.q_val >> delim
		>> param.bw >> delim
		>> param.min_fold >> delim
		>> param.max_fold >> delim
		>> param.ext_size >> delim
		>> param.fitness  >> delim
		>> param.same;
	return is;
}

template<typename T>
inline T bump(T curr, T min_v, T max_v, T delta, bool decrement) {
	if ((decrement && curr != min_v) || curr == max_v) {
		if (curr - min_v < delta) return min_v; // avoid unsigned underflow
		return curr - delta;
	}
	return min(max_v, curr + delta);
}

#endif
