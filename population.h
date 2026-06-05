#ifndef POPULATION_H_
#define POPULATION_H_
#include <cstdlib>
#include <numeric>
#include "parameter.h"
#include "utils.h"
#include "configuration.h"
constexpr uint POP_AMOUNT = 100U;
constexpr uint GENERATIONS = 100U;
constexpr uint BUDGET = 10000u;
constexpr uint PATIENCE = 10u;
constexpr uint VAR_JANELA = 10;
constexpr double VAR_THRESH = 0.05;

inline double pop_var(const parameter *params, uint n) {
	if (n < 2) return 0.0;
	vector<double> qvals(n), bws(n), min_folds(n), max_folds(n), extsizes(n);
	
	for (uint i = 0; i < n; i++) {
		auto param = params[i];
		qvals[i] = (param.q_val - min_q_val) / q_val_interv;
		bws[i] =  (param.bw - min_bw) / ((double) bw_interv);
		min_folds[i] = (param.min_fold - min_min_fold) / ((double) min_fold_interv);
		max_folds[i] = (param.max_fold - min_max_fold) / ((double) max_fold_interv);
		extsizes[i] = (param.ext_size = min_ext_size) / ((double) ext_size_interv);
	}

	auto stddev = [&](const vector<double>& v) {
		double mean = std::accumulate(v.begin(), v.end(), 0.0) / v.size();
		double sum = 0.0;
		for (double x : v) sum += (x - mean) * (x - mean);
		return std::sqrt(sum / v.size());
	};

	return (stddev(qvals) + stddev(bws) + stddev(min_folds) + stddev(max_folds) + stddev(extsizes)) / 5.0;
}

class population {
protected:
	uint pop_amount;
	double crossover_chance;
	double mutation_rate;
	configuration &config;
	uint patience;
	parameter elitist;

public:
	population(uint pop_amount, configuration &config) : pop_amount(pop_amount),
		crossover_chance(0.6), mutation_rate(0.1), config(config), patience(PATIENCE), elitist(parameter()) {};
	virtual parameter find_best(uint generations) = 0;
};

class serial_population : public population {
private:
	parameter *params;
public:
	serial_population(uint pop_amount, configuration &config) : population(pop_amount, config) {
		params = new parameter[pop_amount];
	};

	~serial_population() {
		delete[] params;
	};

	parameter find_best(uint generations) override;
	void new_generation();
};
#endif