#include "utils.h"

uint get_random(uint n) {
	return rand() % n;
}

uint get_random(uint min, uint max) {
	uint size = max - min + 1;
	return rand() % size + min;
}

double get_random() {
	return (double) rand() / (double) RAND_MAX;
}

double get_random(double min, double max) {
	return get_random() * (max-min) + min;
}
