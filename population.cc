#include "population.h"
#include "parameter.h"

inline double execute_param(parameter& cache, const char *input_fp, const char *macs_dir, const char *sh_cmd, const char *res_fp) {
	string cmd = cache.get_exec_str(input_fp, macs_dir);
	const char *cmd_c = cmd.c_str();
	if (std::system(cmd_c)) {
		std::exit(1);
	}
	if (std::system(sh_cmd)) {
		std::exit(1);
	}
	return cache.get_fitness_from_file(res_fp);
}

inline void population::new_generation(parameter &elitist) {
	for(uint j = 0; j < pop_amount; j++) {
		double random = get_random();
		parameter &cache = params[j];
		if (random <= crossover_chance) {
			cache.crossover(elitist);
			cache.set_same(true);
		}
		random = get_random();
		if (random <= mutation_rate) {
			cache.mutate();
			cache.set_same(true);
		}
	}
}

parameter population::find_best(uint generations) {
	parameter elitist;
	elitist.empty_param();
	uint i, j;
	double fitness;
	bool changed;
	string sh_str = config.get_sh_exec_cmd();
	const char *sh_cmd = sh_str.c_str();
	const char *input_fp = config.get_input_file_path();
	const char *res_fp = config.get_result_path();
	const char *macs_dir = config.get_macs_dir();
	std::ofstream csv("generations.csv");
	csv << "Generation;Fitness\n";
	for(i = 0; i < generations; i++) {
		changed = false;
		for(j = 0; j < pop_amount; j++) {
			parameter &cache = params[j];
			if(cache.get_same()) continue;
			cache.set_same(false);
			fitness = execute_param(cache, input_fp, macs_dir, sh_cmd, res_fp);
			if (fitness > elitist.get_fitness()) {
				parameter::replace(elitist, cache);
				changed = true;
			}
		}

        csv << i << ';' << elitist.get_fitness() << std::endl;

		if (!changed) {

		}
	}
}
