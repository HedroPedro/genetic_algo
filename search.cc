#include "search.h"

std::ofstream search::open_run_csv(const string &filename, const string &header) {
	auto &chk = config.chck();
	bool resuming = chk.start_generation > 0;

	bool needs_header = !resuming || !std::ifstream(filename).good();

	std::ofstream csv(filename, resuming ? std::ios::app : std::ios::trunc);
	if (needs_header) {
		csv << header << '\n';
	}
	return csv;
}