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
constexpr uint PATIENCE = 4u;

class population {
protected:
	parameter *params;
	uint pop_amount;
	double crossover_chance;
	double mutation_rate;
	configuration &config;
	uint patience;
public:
	population(uint pop_amount, configuration &config): params(new parameter[pop_amount]), pop_amount(pop_amount),
		crossover_chance(0.6), mutation_rate(0.1), config(config), patience(PATIENCE){};
	~population() {delete params;};
	virtual parameter find_best(uint generations);
	void new_generation();
    void new_generation(uint start);
};

class thread_population : public population {
private:
	uint n_threads;
public:
	thread_population(uint pop_amount, configuration &config, uint n_threads) : population(pop_amount, config), n_threads(n_threads) {};
	parameter find_best(uint generations) override;
	void new_gen(uint start, uint amount);
};
#endif