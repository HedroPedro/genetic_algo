#ifndef POPULATION_H_
#define POPULATION_H_
#include <cstdlib>
#include <vector>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include "parameter.h"
#include "utils.h"
#include "configuration.h"
using std::vector;
using std::string;
constexpr uint POP_AMOUNT = 100U;
constexpr uint GENERATIONS = 100U;

class thread_pop_config;

class population {
protected:
    parameter *params;
    uint pop_amount;
    double crossover_chance;
    double mutation_rate;
    configuration &config;
public:
    population(uint pop_amount, configuration &config) : params(new parameter[pop_amount]), pop_amount(pop_amount),
        crossover_chance(0.6), mutation_rate(0.05), config(config) {};
    ~population() {delete params;};
    virtual parameter find_best(uint generations);
    void new_generation(parameter &elitist);
};

class population_threaded : population {
private:
    uint n_thread;
    static void find_best_slice(thread_pop_config &t_config);
public:
    population_threaded(uint pop_amount, configuration &config, uint n_thread) : population(pop_amount, config), n_thread(n_thread) {};
    parameter find_best(uint generation) override;
};

class thread_pop_config {
public:
    uint starting_index;
    uint pop_amount;
    uint id;
    parameter &best;
    configuration &config;
    parameter *pop;
    const char *macs_dir;
    const char *input_fp;
    const char *res_dir;

    thread_pop_config(uint starting_index, uint pop_amount, uint id, parameter &best, configuration &config, parameter *pop,
    const char *macs_dir,
    const char *input_fp,
    const char *res_dir) : starting_index(starting_index), pop_amount(pop_amount), id(id), best(best), config(config), pop(pop), macs_dir(macs_dir), 
    input_fp(input_fp), res_dir(res_dir) {};
};

#endif