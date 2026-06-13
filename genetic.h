#ifndef GENETIC_H_
#define GENETIC_H_
#include <cstdlib>
#include <numeric>
#include <chrono>
#include "parameter.h"
#include "utils.h"
#include "configuration.h"
constexpr uint POP_AMOUNT = 100U;
constexpr uint GENERATIONS = 1000U;
constexpr uint PATIENCE = 35u;

class genetic {
protected:
	uint pop_amount;
	double crossover_chance;
	double mutation_rate;
	configuration &config;
	uint patience;
	parameter elitist;

public:
	genetic(uint pop_amount, configuration &config) : pop_amount(pop_amount),
		crossover_chance(0.6), mutation_rate(0.05), config(config), patience(PATIENCE), elitist(parameter()) {};
	virtual parameter find_best(uint generations) = 0;
};

class serial_genetic : public genetic {
private:
	parameter *params;
public:
	serial_genetic(uint pop_amount, configuration &config) : genetic(pop_amount, config) {
		params = new parameter[pop_amount];
	};

	~serial_genetic() {
		delete[] params;
	};

	parameter find_best(uint generations) override;
	void new_generation();
};
#endif