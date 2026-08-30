#include "main.h"

int main(int argc, char *argv[]) {
	configuration config;
	search *alg = parse_cmd(argc, argv, config);

	if(!config.rnd_is_set()) {
		std::random_device rd;
		auto seed = rd();
		config.chck().mt.seed(seed);
		std::clog << "Not RNG State found using: " << seed << '\n';
	} else {
		std::clog << "Using RNG state from checkpoint.\n";
	}

	rng::init(config.chck().mt);

	auto cmd = alg->find_best();
	cout << cmd << '\n';
	config.chck().f_checkpoint.close();

	std::remove(config.checkpoint_name().c_str());

	delete alg;
	return 0;
}
