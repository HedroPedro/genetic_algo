#ifndef PARAMETERS_H_
#define PARAMETERS_H_
#include <sstream>
#include <algorithm>
#include "utils.h"
using std::min;
using std::max;
using std::swap;

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

struct _parameter {
    uint bw;
    uint min_fold, max_fold;
    uint ext_size;
    double q_val;
    double fitness;
    bool same;
};

class parameter {
private:
    double q_val;
    uint bw;
    uint min_fold, max_fold;
    uint ext_size;
    double fitness;
    bool same;
public:
    parameter(void) : q_val(get_random(min_q_val, max_q_val)), bw(get_random(min_bw, max_bw)), 
        min_fold(get_random(min_min_fold, max_min_fold)), 
        max_fold(get_random(min_max_fold, max_max_fold)), 
        ext_size(get_random(min_ext_size, max_ext_size)) {
        };

    void crossover(parameter& other);
    void mutate(void);
    std::string get_exec_str(const char *input_fp, const char *macs_dir, const char *other_params);
	double get_fitness_from_file(const char *fp);

    inline double get_fitness(void) {return fitness;};
    inline void set_same(bool same) {this->same = same;}
    inline bool get_same() {return same;}
};

void new__parameter(_parameter &param);
void crossover(_parameter &a, _parameter &b);
void mutate(_parameter &a);

#endif
