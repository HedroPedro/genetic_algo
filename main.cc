#include "main.h"

int main(int argc, char *argv[]) {
	std::srand(time(NULL));
	configuration config(argc, argv);
	#ifndef RANDOM
	genetic *pop;
	serial_genetic p(100, config);
	pop = &p;
	#else
	random_search *pop;
	random_search p(100, config);
	pop = &p;
	#endif
	auto param = pop->find_best(100);
	const char *input_path = config.get_input_file_path();
	const char *macs_dir = config.get_macs_dir();
	const char *params = config.get_other_params();
	auto cmd = param.get_exec_str(input_path, macs_dir, params);
	auto cmd_c = cmd.c_str();
	std::cout << cmd_c << std::endl;
	std::system(cmd_c);
	return 0;
}
