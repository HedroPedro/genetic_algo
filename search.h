#ifndef SEARCH_H_
#define SEARCH_H_
#include <fstream>
#include "parameter.h"
#include "configuration.h"

class search {
protected:
	configuration &config;
	parameter _best;
public:
	search(configuration &config) : config(config) {};
	virtual ~search() = default;
	inline void best(parameter &best) {_best = best;}
	virtual parameter find_best() = 0;
	virtual void write_info() = 0;
	virtual void read_info() = 0;
};

#endif