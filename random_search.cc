#include "random_search.h"

parameter random_search::find_best(uint generations) {
	parameter best;
	auto input_fp = config.get_input_file_path();
	auto sh_str = config.get_sh_exec_cmd();
	auto sh_cmd = sh_str.c_str();
	auto macs_dir = config.get_macs_dir();
	auto other_params = config.get_other_params();
	auto res_path = config.get_result_path();
	std::ostringstream oss;
	oss << "random_generations_" << generations << '_' << n_points << ".csv";
	std::ofstream random_csv(oss.str());
	random_csv << "Iter;Fitness;Params;Time\n";
	for(uint i = 0; i < generations; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		for(uint j = 0; j < n_points; j++) {
			parameter temp;
			temp.execute_param(input_fp, macs_dir, sh_cmd, res_path, other_params);
			if (temp.fitness > best.fitness) {
				best = temp;
			}
		}
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration<double>(end - start).count();

		random_csv << i << ';' << best.get_exec_str(input_fp, macs_dir, other_params) << ';' << best.fitness << ';' << duration << std::endl;
	}

	return best;
}