#ifndef PARAMETERS_H_
#define PARAMETERS_H_
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
    parameter() : q_val(get_random(min_q_val, max_q_val)), bw(get_random(min_bw, max_bw)), 
        min_fold(get_random(min_min_fold, max_min_fold)), 
        max_fold(get_random(min_max_fold, max_max_fold)), 
        ext_size(get_random(min_ext_size, max_ext_size)) {};

    void crossover(parameter& other);

    void mutate(void);
};

#endif
