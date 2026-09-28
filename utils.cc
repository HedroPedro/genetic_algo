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

template<typename T>
T rng::get_random(T n) {
    std::uniform_int_distribution<T> dist(0, n-1);
	return dist(rng::global());
}

template<typename T>
T rng::get_random(T min, T max) {
	std::uniform_real_distribution<T> dist(min, max);
	return dist(rng::global());
}

double rng::get_random() {
	std::uniform_real_distribution<double> dist(0., 1.);
	return dist(rng::global());
}

