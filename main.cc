#include "main.h"

int main(int argc, char *argv[]) {
	configuration config;
	search *alg = parse_cmd(argc, argv, config);

	auto cmd = alg->find_best();
	cout << cmd << '\n';
	config.chck().f_checkpoint.close();

	std::remove(config.checkpoint_name().c_str());

	delete alg;
	return 0;
}
