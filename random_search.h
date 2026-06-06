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
public:
	random_search(uint n_points, configuration& config) : n_points(n_points), config(config) {};
	parameter find_best(uint generations);
};

#endif