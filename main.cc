#include "main.h"

int main(int argc, char *argv[]) {
	configuration config;
	
	search *alg = parse_cmd(argc, argv, config);
	
	auto cmd = alg->find_best();
	cout << cmd << '\n';
	return 0;
}
