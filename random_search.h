#ifndef RANDOM_SEARCH_H_
#define RANDOM_SEARCH_H_
#include <cstdlib>
#include <chrono>
#include "utils.h"
#include "parameter.h"
#include "configuration.h"

class random_search {
private:
	uint n_points;
	configuration &config;
	uint budget;
public:
	random_search(uint n_points, configuration& config, uint budget = 0) : n_points(n_points), config(config) {
		if (budget != 0) {
			this->budget = budget;	
		}
	};
	parameter find_best(uint generations);
};

#endif