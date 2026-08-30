#ifndef CHECKPOINT_H_
#define CHECKPOINT_H_
#include <time.h>
#include "utils.h"
#include "parameter.h"

enum modes {
	GENETIC,
	RANDOM
};

struct checkpoint {
	uint16_t start_generation = 0;
	std::mt19937 mt;
	fstream f_checkpoint;
};

#endif