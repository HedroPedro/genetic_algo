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
	time_t seed = time(NULL);
	fstream f_checkpoint;
};

#endif