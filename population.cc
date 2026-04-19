#include "population.h"

inline double execute_param(parameter& cache, const char *input_fp, const char *macs_dir, const char *sh_cmd, const char *res_fp, const char *other_params) {
	string cmd = cache.get_exec_str(input_fp, macs_dir, other_params);
	const char *cmd_c = cmd.c_str();
	if (std::system(cmd_c)) std::exit(1);
	if (std::system(sh_cmd)) std::exit(1);
	return cache.get_fitness_from_file(res_fp);
}

inline void serial_population::new_generation() {
	for (uint j = 0; j + 1 < pop_amount; j += 2) {
		parameter &a = params[j];
		parameter &b = params[j + 1];

		if (get_random() <= crossover_chance) {
			a.crossover(b);
		} else {
			uint index = get_random(2);
			params[j+index] = parameter();
		}
		
		if (get_random() <= mutation_rate) a.mutate();
		if (get_random() <= mutation_rate) b.mutate();
	}

	if (pop_amount & 1) {
		if (get_random() <= mutation_rate) params[pop_amount - 1].mutate();
	}
}

parameter serial_population::find_best(uint generations) {
	parameter elitist;
	uint j;
	double fitness;
	bool changed;
	string sh_str = config.get_sh_exec_cmd();
	const char *sh_cmd = sh_str.c_str();
	const char *input_fp = config.get_input_file_path();
	const char *macs_dir = config.get_macs_dir();
	const char *res_fp = config.get_result_path();
	const char *other_params = config.get_other_params();
	std::ostringstream oss;
	oss << "generations_" << generations << '_'<< pop_amount << ".csv";
	string name = oss.str();
	const char *name_csv = name.c_str();
	std::ofstream csv(name_csv); 
	uint current_patience = patience;
	csv << "Generation;Fitness;Param" << std::endl;
	for(uint i = 0; i < generations; i++) {
		changed = false;
		for(j = 0; j < pop_amount; j++) {
			parameter &cache = params[j];
			if(cache.get_same()) continue;
			fitness = execute_param(cache, input_fp, macs_dir, sh_cmd, res_fp, other_params);
			cache.set_same(true);
			std::cout << "Gen:" << i << ";Member:" << j << ";Fit:" << fitness << std::endl;
			if (fitness > elitist.get_fitness()) {
				elitist = cache;
				changed = true;
			}
		}

		csv << i << ';' << elitist.get_fitness() << ';'  << elitist.get_exec_str(input_fp, macs_dir, other_params) << std::endl;

		if (!changed) {
			uint index = get_random(pop_amount);
			params[index] = elitist;
			params[index].set_same(true);
			if ((--current_patience) == 0) {
				break;
			}
		} else {
			current_patience = patience;
		}

		new_generation();
	}
	csv.close();
	return elitist;
}
