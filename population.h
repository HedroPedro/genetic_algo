#ifndef POPULATION_H_
#define POPULATION_H_
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "parameter.h"
#include "utils.h"
#include "configuration.h"
using std::vector;
using std::string;
constexpr uint POP_AMOUNT = 100U;
constexpr uint GENERATIONS = 100U;

class population {
private:
    parameter *params;
    uint pop_amount;
    double crossover_chance;
    double mutation_rate;
    configuration config;
public:
    population(uint pop_amount, configuration config): params(new parameter[pop_amount]), pop_amount(pop_amount),
        crossover_chance(0.6), mutation_rate(0.05), config(config) {};
    ~population() {delete params;};
    parameter find_best(uint generations);
};

#endif