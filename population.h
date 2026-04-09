#ifndef POPULATION_H_
#define POPULATION_H_
#include <cstdlib>
#include <vector>
#include <thread>
#include <fstream>
#include <sstream>
#include "parameter.h"
#include "utils.h"
#include "configuration.h"
using std::vector;
using std::string;
using std::thread;
constexpr uint POP_AMOUNT = 100U;
constexpr uint GENERATIONS = 100U;

class population {
protected:
	parameter *params;
	uint pop_amount;
	double crossover_chance;
	double mutation_rate;
	const char *sh_cmd;
	const char *input_fp;
	const char *res_fp;
	const char *macs_dir;
public:
	population(uint pop_amount, configuration &config): params(new parameter[pop_amount]), pop_amount(pop_amount),
		crossover_chance(0.6), mutation_rate(0.1) {
			auto sh_str = config.get_sh_exec_cmd();
			sh_cmd = sh_str.c_str();
			input_fp  = config.get_input_file_path();
			res_fp = config.get_result_path();
		};
	~population() {delete params;};
	virtual parameter find_best(uint generations);
	void new_generation(parameter &elitist);
    void new_generation_start_at(uint start, parameter &elitist);
};

class thread_population : public population {
private:
	uint n_threads;
	void new_gen(uint start, uint amount, parameter &best);
public:
	thread_population(uint pop_amount, configuration &config, uint n_threads) : population(pop_amount, config), n_threads(n_threads) {};
	parameter find_best(uint generations) override;
};

#endif