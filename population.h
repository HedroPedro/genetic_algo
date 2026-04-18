#ifndef POPULATION_H_
#define POPULATION_H_
#include <cstdlib>
#include <sstream>
#include "parameter.h"
#include "utils.h"
#include "configuration.h"
#include "worker.h"
using std::vector;
using std::string;
constexpr uint POP_AMOUNT = 100U;
constexpr uint GENERATIONS = 100U;
constexpr uint PATIENCE = 65u;

class population {
protected:
	uint pop_amount;
	double crossover_chance;
	double mutation_rate;
	configuration &config;
	uint patience;
public:
	population(uint pop_amount, configuration &config) : pop_amount(pop_amount),
		crossover_chance(0.6), mutation_rate(0.1), config(config), patience(PATIENCE){};
	virtual parameter find_best(uint generations) = 0;
};

class serial_population : public population {
private:
	parameter *params;
public:
	serial_population(uint pop_amount, configuration &config) : population(pop_amount, config) {
		params = new parameter[pop_amount];
	};

	~serial_population() {
		delete[] params;
	}

	parameter find_best(uint generations) override;
	void new_generation();
};

class paralel_population : public population {
private:
	uint n_children;
	u_char *shared_map;
	u_char *type_worker;
	_parameter *params;
public:
	paralel_population(uint pop_amount, uint n_children, configuration &config) : population(pop_amount, config) {
		
	};
};

#endif