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

inline double execute_param(parameter& cache, const char *input_fp, const char *macs_dir, const char *sh_cmd,  const char *res_fp, uint id) {
	std::ostringstream os;
	os << cache.get_exec_str(input_fp, macs_dir) << " -n " << id;
	string cmd_str = os.str();
	const char *cmd_c = cmd_str.c_str();
	os.str("");
	os.clear();
	if (std::system(cmd_c)) {
		std::exit(1);
	}
	if (std::system(sh_cmd)) {
		std::exit(1);
	}
	return cache.get_fitness_from_file(res_fp, id);
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

parameter population::find_best(uint generations) {
	parameter elitist;
	elitist.empty_param();
	uint i, j;
	double fitness;
	bool changed;
	string sh_str = config.get_sh_exec_cmd(0);
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
			fitness = execute_param(cache, input_fp, macs_dir, sh_cmd, res_fp);
			if (fitness > elitist.get_fitness()) {
				parameter::replace(elitist, cache);
				changed = true;
			}
		}

		csv << i << ';' << elitist.get_fitness() << '\n';

		if (!changed) {
			j = get_random(pop_amount);
			parameter::replace(params[j], elitist);
		}

		new_generation(elitist);
		elitist.empty_param();
	}

	csv.close();
	return elitist;
}

parameter population_threaded::find_best(uint generations) {
    parameter better_elitist;
    better_elitist.empty_param();

    std::vector<parameter> elitists;
    std::vector<std::thread> threads;
    std::vector<thread_pop_config> t_vec;

    elitists.resize(n_thread);
    threads.resize(n_thread);
    t_vec.reserve(n_thread);

    uint amount_per_slice = pop_amount / n_thread;
    const char *macs_dir = config.get_macs_dir();
    const char *input_fp = config.get_input_file_path();
    const char *res_fp   = config.get_result_path();
    const uint last_iter_id = n_thread + 1;
    bool changed;

    for (size_t i = 0; i < n_thread; i++) {
        t_vec.push_back(thread_pop_config(
            amount_per_slice * i, amount_per_slice, i + 1,
            elitists[i],
            config, params, macs_dir, input_fp, res_fp
        ));
    }

    for (uint i = 0; i < generations; i++) {
        changed = false;

        for (auto &e : elitists) e.empty_param();

        for (uint j = 0; j < n_thread; j++)
            threads[j] = std::thread(find_best_slice, std::ref(t_vec[j]));

        for (auto &thread : threads)
            if (thread.joinable()) thread.join();

        for (uint j = amount_per_slice * n_thread; j < pop_amount; j++) {
            parameter &cache = params[j];
            string sh_str = config.get_sh_exec_cmd(last_iter_id);
            const char *sh_cmd = sh_str.c_str();
            double fitness = execute_param(cache, input_fp, macs_dir, sh_cmd, res_fp, last_iter_id);
            if (fitness > better_elitist.get_fitness()) {
                parameter::replace(better_elitist, cache);
                changed = true;
            }
        }

        for (auto &param : elitists) {
            if (param.get_fitness() > better_elitist.get_fitness()) {
                parameter::replace(better_elitist, param);
                changed = true;
            }
        }

        if (!changed) {
            uint j = get_random(pop_amount);
            parameter::replace(params[j], better_elitist);
        }

        new_generation(better_elitist);
    }

    return better_elitist;
}

void population_threaded::find_best_slice(thread_pop_config &t_config) {
	uint starting_index = t_config.starting_index;
	uint total = t_config.pop_amount+starting_index;
	uint id = t_config.id;
	parameter *pop = t_config.pop;
	configuration &config = t_config.config;
	parameter &elitist = t_config.best;
	string sh_str = config.get_sh_exec_cmd(t_config.id);
	const char *sh_cmd = sh_str.c_str();
	const char *input_fp = t_config.input_fp;
	const char *macs_dir = t_config.macs_dir;
	const char *res_dir = t_config.res_dir;
	double fitness;
	
	for(uint i = starting_index; i < total; i++) {
		parameter &cache = pop[i];
		fitness = execute_param(cache, input_fp, macs_dir, sh_cmd, res_dir, id);
		if (fitness > elitist.get_fitness()) {
			parameter::replace(elitist, cache);
		}
	}
}