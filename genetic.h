#ifndef GENETIC_H_
#define GENETIC_H_
#include <cstdlib>
#include <numeric>
#include <chrono>
#include "parameter.h"
#include "search.h"
#include "utils.h"
#include "configuration.h"

class genetic : public search {
protected:
	double crossover_chance;
	double mutation_rate;
	uint patience;
	uint current_patience;
	uint16_t pop_amount;
	uint16_t generations;
	parameter *params;
public:
	genetic(configuration &config) :
		search(config), crossover_chance(0.6), mutation_rate(0.05) {
			pop_amount = config.population();
			generations = config.generations();
			patience = config.patience();
			if (patience == 0) patience = generations;
			current_patience = patience;
		};
	
	void write_info() override;
	void read_info()  override;
};

class serial_genetic : public genetic {
public:
	serial_genetic(configuration &config) : genetic(config) {
		params = new parameter[pop_amount];
	};

	inline ~serial_genetic() {
		delete[] params;
	};

	parameter find_best() override;
	void new_generation();
};
#endif
