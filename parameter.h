#ifndef PARAMETERS_H_
#define PARAMETERS_H_
#include <sstream>
#include <fstream>
#include "utils.h"
using std::min;
using std::max;

constexpr double min_q_val = 0.001;
constexpr double max_q_val = 0.1;
constexpr uint min_bw = 150U;
constexpr uint max_bw = 450U;
constexpr uint min_min_fold = 2U;
constexpr uint max_min_fold = 10U;
constexpr uint min_max_fold = 15U;
constexpr uint max_max_fold = 50U;
constexpr uint min_ext_size = 100U;
constexpr uint max_ext_size = 300U;

class parameter {
private:
    double q_val;
    uint bw;
    uint min_fold, max_fold;
    uint ext_size;
    double fitness = 0.;
public:
    parameter(void) : q_val(get_random(min_q_val, max_q_val)), bw(get_random(min_bw, max_bw)), 
        min_fold(get_random(min_min_fold, max_min_fold)), 
        max_fold(get_random(min_max_fold, max_max_fold)), 
        ext_size(get_random(min_ext_size, max_ext_size)) {};

    inline void empty_param() {
	    this->q_val = 0.;
	    this->bw = 0;
	    this->min_fold = 0;
	    this->max_fold = 0;
	    this->ext_size = 0;
    };

    static inline void replace(parameter& to_replace, parameter& replacer) {
        to_replace.q_val = replacer.q_val;
        to_replace.bw = replacer.bw;
        to_replace.min_fold = replacer.min_fold;
        to_replace.max_fold = replacer.max_fold;
        to_replace.ext_size = replacer.ext_size;
        to_replace.fitness = replacer.fitness;
    };

    void crossover(parameter& other);
    void mutate(void);
    std::string get_exec_str(const char *input_fp, const char *macs_dir);
    double get_fitness_from_file(const char *fp);
    double get_fitness_from_file(const char *fp, uint n);

    inline double get_fitness(void) {return fitness;};
	
};

#endif
