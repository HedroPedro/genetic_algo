#include "population.h"

inline double execute_param(parameter& cache, const char *input_fp, const char *macs_dir, const char *sh_cmd, const char *res_fp) {
	string cmd = cache.get_exec_str(input_fp, macs_dir);
	const char *cmd_c = cmd.c_str();
	if (std::system(cmd_c)) std::exit(1);
	if (std::system(sh_cmd)) std::exit(1);
	return cache.get_fitness_from_file(res_fp);
}

inline void population::new_generation() {
    for (uint j = 0; j + 1 < pop_amount; j += 2) {
        parameter &a = params[j];
        parameter &b = params[j + 1];

        if (get_random() <= crossover_chance) a.crossover(b);
        
        if (get_random() <= mutation_rate) a.mutate();
        if (get_random() <= mutation_rate) b.mutate();
    }

    if (pop_amount & 1) {
        if (get_random() <= mutation_rate) params[pop_amount - 1].mutate();
    }
}

inline void population::new_generation(uint s) {
    for (uint i = s; i + 1 < pop_amount; i += 2) {
        parameter &a = params[i];
        parameter &b = params[i + 1];

        if (get_random() <= crossover_chance) a.crossover(b);
        
        if (get_random() <= mutation_rate) a.mutate();
        if (get_random() <= mutation_rate) b.mutate();
    }

    if ((pop_amount - s) & 1) {
        if (get_random() <= mutation_rate) params[pop_amount - 1].mutate();
    }
}

inline void thread_population::new_gen(uint offset, uint amount) {
    uint end = offset + amount;
    for (uint i = offset; i + 1 < end; i += 2) {
        parameter &a = params[i];
        parameter &b = params[i + 1];

        if (get_random() <= crossover_chance) {
            a.crossover(b);
        }
        if (get_random() <= mutation_rate) a.mutate();
        if (get_random() <= mutation_rate) b.mutate();
    }

    if (amount & 1) {
        if (get_random() <= mutation_rate)
            params[end - 1].mutate();
    }
}

parameter population::find_best(uint generations) {
	parameter elitist;
	elitist.empty_param();
	uint j;
	double fitness;
	bool changed;
	string sh_str = config.get_sh_exec_cmd();
	const char *sh_cmd = sh_str.c_str();
	const char *input_fp = config.get_input_file_path();
	const char *macs_dir = config.get_macs_dir();
	const char *res_fp = config.get_result_path();
	std::ostringstream oss;
	oss << "generations_" << generations << '_'<< pop_amount << ".csv";
	string name = oss.str();
	const char *name_csv = name.c_str();
	std::ofstream csv(name_csv); 
	csv << "Generation;Fitness" << std::endl;
	for(uint i = 0; i < generations; i++) {
		changed = false;
		for(j = 0; j < pop_amount; j++) {
			parameter &cache = params[j];
			if(cache.get_same()) continue;

			cache.set_same(true);
			fitness = execute_param(cache, input_fp, macs_dir, sh_cmd, res_fp);
			if (fitness > elitist.get_fitness()) {
				parameter::replace(elitist, cache);
				changed = true;
			}
			std::cout << "Gen:" << i << ";Member:" << j << ";Fit:" << fitness;
		}

        csv << i << ';' << elitist.get_fitness() << std::endl;

		if (!changed) {
			uint index = get_random(pop_amount);
			parameter::replace(params[index], elitist);
			params[index].set_same(true);
		}

		new_generation();
	}
	csv.close();
	return elitist;
}

parameter thread_population::find_best(uint generations) {
    parameter elitist;
	elitist.empty_param();
	vector<thread> threads(n_threads);
	uint slices = pop_amount/n_threads;
	uint j;
	bool changed;
	double fitness;
	string sh_str = config.get_sh_exec_cmd();
	const char *sh_cmd = sh_str.c_str();
	const char *input_fp = config.get_input_file_path();
	const char *macs_dir = config.get_macs_dir();
	const char *res_fp = config.get_result_path();
	std::ostringstream oss;
	oss << "generations_" << generations << '_'<< pop_amount << ".csv";
	string name = oss.str();
	const char *name_csv = name.c_str();
	std::ofstream csv(name_csv);
	csv << "Generation;Fitness" << std::endl;
	for (uint i = 0; i < generations; i++) {
		changed = false;
		for(j = 0; j < pop_amount; j++) {
			parameter &cache = params[j];
			if(cache.get_same()) continue;
			cache.set_same(true);
			fitness = execute_param(cache, input_fp, macs_dir, sh_cmd, res_fp);
			if (fitness > elitist.get_fitness()) {
				parameter::replace(elitist, cache);
				changed = true;
			}
			std::cout << "Gen:" << i << ";Member:" << j << ";Fit:" << fitness;
		}

		csv << i << ';' << elitist.get_fitness() << std::endl;

		if (!changed) {
			uint index = get_random(pop_amount);
			parameter::replace(params[index], elitist);
			params[index].set_same(true);
		}

		for (j = 0; j < n_threads; j++) {
			threads[j] = thread(&thread_population::new_gen, this, j*slices, slices);
		}

		for (thread &t : threads) {
			if(t.joinable()) t.join();
		}

		new_generation(n_threads*slices);

	}
	csv.close();
	return elitist;
}
