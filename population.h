#ifndef POPULATION_H_
#define POPULATION_H_
#include <cstdlib>
#include <vector>
#include "parameter.h"
#include "utils.h"
using std::vector;
constexpr uint POP_AMOUNT = 100U;
constexpr uint GENERATIONS = 100U;

class population {
private:
    parameter *params;
    parameter elitist;
    double crossover_chance;
    double mutation_rate;
public:
    population(void) : params(new parameter[POP_AMOUNT]), 
        crossover_chance(0.6), mutation_rate(0.05) {};
    ~population() {delete params;};
    parameter find_best(uint generations = GENERATIONS);
};

#endif