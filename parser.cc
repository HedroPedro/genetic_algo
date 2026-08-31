#include "parser.h"

void print_help() {
	cout << "./alg [<configuration file> or <checkpoint>]\n";
	cout << "Command Line Flags:\n";
	cout << "-h: Print this information\n";
	cout << "Checkpoint MUST have extension as .chk\n";
	cout << "== CONFIGURATION PARAMETERS ==\n";
	cout << EXPERIMENT_ATTR << "- Path for .bam file (Required)\n";
	cout << SHELL_ATTR << "- Path for the shell pipeline after (Required)\n";
	cout << TOMTOM_RES_ATTR << "- Path for the TOMTOM generated tsv (Required)\n";
	cout << OTHER_PARAMS_ATTR << "- Extra parameters like genome size or broad (Defaults to nothing)\n";
	cout << MACS_DIR_ATTR << "- Name for the MACS directory (Defaults to res)\n";
	cout << CHECKPOINT_NAME_ATTR << "- Path to checkpoint file (Defaults 'checkpoint.chk')" << std::endl;
	cout << MODE_ATTR << "- Select mode for algorithm (Defaults to " << MODE_GENETIC_ATTR << "). Expected values: " << MODE_GENETIC_ATTR << " or " << MODE_RANDOM_ATTR << '\n';
	cout << POP_ATTR << "- Population size or amount for points for each iteration (Defaults to 100)\n";
	cout << ITER_ATTR << "- Amount of iterations (Defaults to 100)\n";
	cout << "== GENETIC ALGORITHM SPECIFIC ==\n";
	cout << PATIENCE_ATTR << "- Defines the amount of generations before stopping. If zero it wil assume PATIENCE=GENERATIONS (Defaults to 6)" << std::endl;
	cout << "== RANDOM SERACH SPECIFIC ==\n";
	cout << BUDGET_ATTR << "- Budget for Random Search. If zero it will calculte via Population x Iterations (Defaults to 0)\n";
	cout << "== FILE EXEMPLE ==\n";
	cout << EXPERIMENT_ATTR << "=experiment.bam\n";
	cout << SHELL_ATTR << "=run.sh\n";
	cout << TOMTOM_RES_ATTR << "=tomtom_out/tomtom.tsv\n";
	cout << MACS_DIR_ATTR << "=res" << std::endl;
}

search *parse_cmd(int argc, char *argv[], configuration &config) {
	if (argc == 1) {
		cout << "Try ./alg [-h or config file or checkpoint]\n";
		exit(1);
	}

	if(strcmp(argv[1], HELP_FLAG) == 0) {
		print_help();
		exit(0);
	}

	auto arg = string(argv[1]);
	auto len = arg.length();
	if (len < 4) {
		auto alg = parse_config_file(argv[1], config);
		return alg;
	}

	auto last_pos = arg.length();
	auto extension = arg.substr(last_pos-4, 4);
	if (extension.compare(CHECKPOINT_EXTENSION) == 0)
		return parse_checkpoint(argv[1], config);

	return parse_config_file(argv[1], config);
}

bool is_file_not_valid(string &fp) {
	std::ifstream ifs(fp);

	if (!ifs) {
		cout << "Could not open file: " << fp << '\n';
		return true;
	}

	return false;
}

search *parse_config_file(char const *fp, configuration &config, bool from_ini) {
	std::ifstream conf_if(fp);
	string line;
	bool is_random = false;
	bool any_error = false;
	uint line_pos = 0;
	unordered_set<string> req_set = {EXPERIMENT_ATTR, SHELL_ATTR, TOMTOM_RES_ATTR};
	
	if (!conf_if) {
		cout << "Configuration file " << fp << " was not found\n";
		exit(1);
	}

	auto fp_str = string(fp);
	config.ini_path(fp_str);

	while(getline(conf_if, line)) {
		auto eq_pos = line.find('=');
		line_pos++;
		if (eq_pos == string::npos) {
			cout << "Line: " << line_pos << "; could not find '='"<< std::endl;
			any_error = true;
			continue;
		}

		auto attribute = line.substr(0UL, eq_pos);
		auto value = line.substr(eq_pos+1);

		if (attribute.compare(EXPERIMENT_ATTR) == 0) {
			if (is_file_not_valid(value)) {
				any_error = true;
				continue;
			}

			req_set.erase(EXPERIMENT_ATTR);
			config.experiment(value);
			continue;
		}

		if (attribute.compare(SHELL_ATTR) == 0) {
			if (is_file_not_valid(value)) {
				any_error = true;
				continue;
			}

			req_set.erase(SHELL_ATTR);
			config.sh_path(value);
			continue;
		}

		if (attribute.compare(TOMTOM_RES_ATTR) == 0) {
			req_set.erase(TOMTOM_RES_ATTR);
			config.result_path(value);
			continue;
		}

		if (attribute.compare(MACS_DIR_ATTR) == 0) {
			config.macs_dir(value);
			continue;
		}

		if (attribute.compare(OTHER_PARAMS_ATTR) == 0) {
			config.other_params(value);
			continue;
		}

		if (attribute.compare(POP_ATTR) == 0) {
			auto i = stoul(value);
			config.population(i);
			continue;
		}

		if (attribute.compare(ITER_ATTR) == 0) {
			auto i = stoul(value);
			config.generations(i);
			continue;
		}

		if (attribute.compare(PATIENCE_ATTR) == 0) {
			auto i = stoul(value);
			config.generations(i);
			continue;
		}

		if (attribute.compare(BUDGET_ATTR) == 0) {
			auto i = stoul(value);
			config.budget(i);
			continue;
		}

		if (attribute.compare(CHECKPOINT_NAME_ATTR) == 0) {
			config.checkpoint_name(value);
			continue;
		}
		
		if (attribute.compare(MODE_ATTR) == 0) {
			if(value.compare(MODE_GENETIC_ATTR) == 0) {
				is_random = false;
				continue;
			}

			if(value.compare(MODE_RANDOM_ATTR) == 0) {
				is_random = true;
				continue;
			}

			cout << "Line: " << line_pos << "; invalid value " << value << ". Guessing Genetic...\n";
			continue;
		}

		cout << "UNKNOWN ARGUMENT. IGNORING...\n";
	}

	
	if (!req_set.empty()) {
		any_error = true;
		for (const auto & key : req_set) {
			cout << "Required key: " << key << " not found!\n";
		}
	}

	if (any_error) {
		conf_if.close();
		exit(1);
	}

	auto &chk = config.chck();

	if (!chk.f_checkpoint.is_open()) {
		chk.f_checkpoint.open(config.checkpoint_name(),
			std::ios::in | std::ios::out | std::ios::trunc);
	}

	if (from_ini) {
		std::random_device rd;
		auto seed = rd();
		config.chck().mt.seed(seed);
		rng::init(config.chck().mt);
	}

	//Create the corresponding algorithm
	if (is_random) {
		return new random_search(config);
	}

	std::cout << "Parsed" << std::endl;
	return new serial_genetic(config);
}

search *parse_checkpoint(char const *fp, configuration &config) {
	auto &chk = config.chck();
	chk.f_checkpoint = fstream(fp);
	auto &chck_fs = chk.f_checkpoint;
	string line;
	
	if(!chck_fs) {
		cout << "Could not open checkpoint file " << fp << '\n';
		exit(1);
	}

	getline(chck_fs, line);
	auto ini_fp = line.c_str();

	// Seed and random
	chck_fs >> chk.mt >> std::ws;

	rng::init(chk.mt);

	auto alg = parse_config_file(ini_fp, config, false);

	//Best parameter
	parameter param;
	chck_fs >> param >> std::ws;
	alg->best(param);

	// Current Generation/Iteration 
	getline(chck_fs, line);
	chk.start_generation = stoul(line);

	alg->read_info();

	config.set_true_rnd_is_set();

	return alg;
}

/* 
CHECKPOINT FILE MUST BE:
INI FILE PATH
State
_best PARAMS
START_GENERATION
<IF TYPE==GENETIC>
[...params]
<ELSE>
BUDGET
<END-IF>
*/