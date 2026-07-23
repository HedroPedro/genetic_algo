#ifndef RANDOM_SEARCH_H_
#define RANDOM_SEARCH_H_
#include <cstdlib>
#include <chrono>
#include "utils.h"
#include "parameter.h"
#include "configuration.h"
#include "search.h"

class random_search : public search {
private:
	uint16_t n_points;
	uint16_t budget;
public:
	random_search(configuration& config) : search(config) {
		n_points = config.population();
		budget = config.budget();
	};

	parameter find_best();

	void write_info() override;
	void read_info() override;
};

#endif