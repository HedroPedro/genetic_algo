#include "genetic.h"
using rng::get_random;

void genetic::write_info() {
	auto &chk = config.chck();
	string tmp_path = config.checkpoint_name() + ".tmp";

	std::ofstream tmp(tmp_path, std::ios::trunc);

	if (!tmp) {
		std::cerr << "Unable to create " << tmp_path << ". Closing program" << std::endl;
		std::exit(1);
	}

	tmp << config.ini_path() << '\n'
	    << chk.mt << '\n'
	    << _best << '\n'
	    << chk.start_generation << '\n'
		<< current_patience << '\n';
	
	for (uint16_t i = 0; i < pop_amount; i++) {
		tmp << params[i] << '\n';
	}
	
	tmp.flush();
	tmp.close();

	chk.f_checkpoint.close();
	std::rename(tmp_path.c_str(), config.checkpoint_name().c_str());
	chk.f_checkpoint.open(config.checkpoint_name(), std::ios::in | std::ios::out);
}

void genetic::read_info() {
	auto &chk = config.chck();
	auto &fs = chk.f_checkpoint;

	if(!fs) return;

	string line;
	getline(fs, line);
	current_patience = stoul(line);

	for(uint i = 0; i < pop_amount; i++) {
		fs >> params[i] >> std::ws;
	}

}

inline void genetic::new_generation() {
	uint16_t pop_amount = config.population();
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

		if (a == _best) a.same = true;
		if (b == _best) b.same = true;
	}

	if (pop_amount & 1) {
		if (get_random() <= mutation_rate) params[pop_amount - 1].mutate();
	}
}

parameter serial_genetic::find_best() {
	uint j;
	bool changed;
	const auto &sh_str = config.sh_exec_cmd();
	const auto &input_fp = config.experiment();
	const auto &macs_dir = config.macs_dir();
	const auto &res_fp = config.result_path();
	const auto &other_params = config.other_params();
	auto &chk = config.chck();
	ostringstream oss;

	oss << "generations_" << generations << '_'<< pop_amount << ".csv";
	auto csv = open_run_csv(oss.str(), "Generation;Fitness;Param;Budget;Time");

	for(uint i = chk.start_generation; i < generations; i++) {
		changed = false;
		uint budget = 0;
		auto start = std::chrono::high_resolution_clock::now();
		for(j = 0; j < pop_amount; j++) {
			parameter &cache = params[j];
			if(cache.same) continue;
			cache.execute_param(input_fp, macs_dir, sh_str, res_fp, other_params);
			budget++;
			if (cache.fitness > _best.fitness) {
				_best = cache;
				changed = true;
			}
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration<double>(end - start).count();

		csv << i << ';' << _best.fitness << ';'  << _best.get_exec_str(input_fp, macs_dir, other_params) 
			<< ';' << budget <<  ';' << elapsed << std::endl;

		if (!changed) {
			uint index = get_random(pop_amount);
			params[index] = _best;
			params[index].same = true;
			if ((--current_patience) == 0) {
				break;
			}
		} else {
			current_patience = patience;
		}

		new_generation();

		chk.start_generation++;
		write_info();
	}
	csv.close();
	return _best;
}

parameter paralel_genetic::find_best() {
	uint j;
	bool changed;
	const auto &sh_str = config.sh_exec_cmd();
	const auto &input_fp = config.experiment();
	const auto &macs_dir = config.macs_dir();
	const auto &res_fp = config.result_path();
	const auto &other_params = config.other_params();
	auto &chk = config.chck();
	ostringstream oss;
	vector<std::thread> threads(n_threads);
	uint stride = pop_amount / n_threads;

	oss << "paralel_generations_" << generations << '_'<< pop_amount << ".csv";
	auto csv = open_run_csv(oss.str(), "Generation;Fitness;Param;Budget;Time");
	auto lambda = [&](uint id, uint start, uint end){
			const auto &sh_str = config.sh_exec_cmd(id);
			uint local_budget = 0;
			parameter local_best = _best;
	
			for(uint i = start; i < end; i++) {
				parameter &cache = params[i];
				if(cache.same) continue;
				cache.execute_param(input_fp, macs_dir, sh_str, res_fp, other_params, id);
				local_budget++;
				if (cache.fitness > local_best.fitness) local_best = cache;
			}

		local_bests[id] = local_best;
		local_budgets[id] = local_budget;
	};

	for(uint i = chk.start_generation; i < generations; i++) {
		uint budget = 0;
		changed = false;
		auto start = std::chrono::high_resolution_clock::now();

		for (uint j = 0; j < n_threads; j++)
			threads[j] = thread(lambda, j, j * stride, j == n_threads-1 ? pop_amount : ((j+1) * pop_amount));			

		for (uint j = 0; j < n_threads; j++)
			threads[j].join();
		
		for (uint j = 0; j < n_threads; j++) {
			budget += local_budgets[j];
			if (_best.fitness < local_bests[j].fitness) {
				_best = local_bests[j];
				changed = true;
			}
		}

		auto end = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration<double>(end - start).count();

		csv << i << ';' << _best.fitness << ';'  << _best.get_exec_str(input_fp, macs_dir, other_params) 
			<< ';' << budget <<  ';' << elapsed << std::endl;

		if (!changed) {
			uint index = get_random(pop_amount);
			params[index] = _best;
			params[index].same = true;
			if ((--current_patience) == 0) break;
		} else {
			current_patience = patience;
		}

		new_generation();
		write_info();
	}

	csv.close();
	return _best;
}
