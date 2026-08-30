#include "utils.h"

namespace {
	std::mt19937 *g_engine = nullptr;	
}

void rng::init(std::mt19937& engine) {
	g_engine = &engine;
}

std::mt19937 &rng::global() {
	return *g_engine;
}

uint rng::get_random(uint n) {
    std::uniform_int_distribution<uint> dist(0, n-1);
	return dist(rng::global());
}

uint rng::get_random(uint min, uint max) {
	std::uniform_int_distribution<uint> dist(min, max);
	return dist(rng::global());
}

double rng::get_random() {
	std::uniform_real_distribution<double> dist(0., 1.);
	return dist(rng::global());
}

double rng::get_random(double min, double max) {
	std::uniform_real_distribution<double> dist(min, max);
	return dist(rng::global());
}

