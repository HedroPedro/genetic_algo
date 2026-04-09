#include "population.h"

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
		}
		random = get_random();
		if (random <= mutation_rate) {
			cache.mutate();
		}
	}
}

inline void population::new_generation_start_at(uint s, parameter &elitist) {
	for(uint i = s; s < pop_amount; i++) {
		double random = get_random();
		parameter &cache = params[i];
		if (random <= crossover_chance) {
			cache.crossover(elitist);
		}
		random = get_random();
		if (random <= mutation_rate) {
			cache.mutate();
		}
	}
}

parameter population::find_best(uint generations) {
	parameter elitist;
	elitist.empty_param();
	uint j;
	double fitness;
	bool changed;
	std::ofstream csv("generations.csv");
	csv << "Generation;Fitness\n" << std::endl;
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
		}

        csv << i << ';' << elitist.get_fitness() << std::endl;

		if (!changed) {
			uint index = get_random(pop_amount);
			parameter::replace(params[index], elitist);
			params[index].set_same(false);
		}

		new_generation(elitist);
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
	std::ofstream csv("generations.csv");
	csv << "Generation;Fitness\n" << std::endl;
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
		}

		csv << i << ';' << elitist.get_fitness() << std::endl;

		if (!changed) {
			uint index = get_random(pop_amount);
			parameter::replace(params[index], elitist);
			params[index].set_same(false);
		}

		for (j = 0; j < n_threads; j++) {
			threads[j] = thread(&thread_population::new_gen, this, j*slices, slices, std::ref(elitist));
		}

		for (thread &t : threads) {
			if(t.joinable()) t.join();
		}

		new_generation_start_at(n_threads*slices, elitist);

	}
	csv.close();
	return elitist;
}

void thread_population::new_gen(uint offset, uint amount, parameter &elitist) {
	uint end = offset+amount;
	for (uint i = offset; i < end; i++) {
		double random = get_random();
		parameter &cache = params[i];
		if (random <= crossover_chance) cache.crossover(elitist);
		
		random = get_random();
		if (random <= mutation_rate) cache.mutate();
	}
}