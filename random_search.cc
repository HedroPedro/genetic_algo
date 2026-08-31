#include "random_search.h"

parameter random_search::find_best() {
	auto generations = config.generations();
	auto &chk = config.chck();

	if (budget == 0) {
		budget = generations * n_points; 
	}
	
	auto input_fp = config.experiment();
	auto sh_str = config.sh_exec_cmd();
	auto sh_cmd = sh_str.c_str();
	auto macs_dir = config.macs_dir();
	auto other_params = config.other_params();
	auto res_path = config.result_path();
	ostringstream oss;
	oss << "random_generations_" << generations << '_' << n_points << ".csv";
	auto random_csv = open_run_csv(oss.str(), "Iter;Fitness;Params;Time");

	auto start = std::chrono::high_resolution_clock::now();

	while(budget != 0) {
		parameter temp;
		temp.execute_param(input_fp, macs_dir, sh_cmd, res_path, other_params);
		if (temp.fitness > _best.fitness) _best = temp;

		budget--;
		if (budget % n_points == 0) {
			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration<double>(end - start).count();
			random_csv << chk.start_generation << ';' 
											<< _best.get_exec_str(input_fp, macs_dir, other_params)
											<< ';' << _best.fitness << ';'
											<< duration
											<< std::endl;
			chk.start_generation++;
			write_info();
			start = std::chrono::high_resolution_clock::now();
		}

	}

	random_csv.close();
	return _best;
}

void random_search::read_info() {
	auto &chk = config.chck();
	auto &fs = chk.f_checkpoint;

	if (!fs) return;
	string line;
	
	if (getline(fs, line)) budget = static_cast<uint16_t>(stoul(line));
}

void random_search::write_info() {
	auto &chk = config.chck();
	string tmp_path = config.checkpoint_name() + ".tmp";

	std::ofstream tmp(tmp_path, std::ios::trunc);
	tmp << config.ini_path() << '\n'
	    << chk.mt() << '\n'
	    << _best << '\n'
	    << chk.start_generation << '\n'
		<< budget << '\n';

	tmp.flush();
	tmp.close();

	chk.f_checkpoint.close();
	std::rename(tmp_path.c_str(), config.checkpoint_name().c_str());
	chk.f_checkpoint.open(config.checkpoint_name(), std::ios::in | std::ios::out);
}