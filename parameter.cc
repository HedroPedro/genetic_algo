#include "parameter.h"

void parameter::crossover(parameter &other) {
	same = false;
	other.same = false;

	swap(min_fold, other.min_fold);
	swap(max_fold, other.max_fold);

	if (min_fold > max_fold) swap(min_fold, max_fold);

	if (other.min_fold > other.max_fold) swap(other.min_fold, other.max_fold);
}

void parameter::mutate(void) {
	same = false;
	uint arg_to_change = rng::get_random(4u);
	bool decrement = rng::get_random() < 0.5;
	switch (arg_to_change) {
		case 0:
			q_val = bump(q_val, min_q_val, max_q_val, rng::get_random(0.001, 0.004), decrement);
		break;
		case 1:
			bw = bump(bw, min_bw, max_bw, rng::get_random(5U, 25U), decrement);
		break;
		case 2:
			min_fold = bump(min_fold, min_min_fold, max_min_fold, rng::get_random(1U, 4U), decrement);
		break;
		case 3:
			max_fold = bump(max_fold, min_max_fold, max_max_fold, rng::get_random(5U, 15U), decrement);
		break;
		default:
			std::cerr << "Why are you here?" << std::endl;
			::exit(1);
		break;
	}

	if(min_fold > max_fold) swap(min_fold, max_fold);
}

string parameter::get_exec_str(const string &input_fp, const string &macs_dir, const string &other_params) {
	ostringstream oss;
	oss << "macs3 callpeak -t " << input_fp << ' ';
	oss << *this << " --verbose 0";
	if(!macs_dir.empty()) oss << " --outdir " << macs_dir;
	if(!other_params.empty()) oss << ' ' << other_params;
	return oss.str();
}

string parameter::get_exec_str(const string &input_fp, const string &macs_dir, const string &other_params, uint id) {
    ostringstream oss;
	oss << "macs3 callpeak -t " << input_fp << ' ';
	oss << *this << " --verbose 0 -n " << id;
	if(!macs_dir.empty()) oss << " --outdir " << macs_dir;
	if(!other_params.empty()) oss << ' ' << other_params;
	return oss.str();
}

double parameter::get_fitness_from_file(const string &fp) {
	std::ifstream stream(fp);
	if(!stream) {
		cout << "Not able to open " << fp << '\n';
		exit(1);
	}

	string str, token;
	std::stringstream ss;

	getline(stream, str);
	getline(stream, str);
	ss = std::stringstream(str);
	for(uint i = 0; i < 5; i++) {
		ss >> token;
	}
	fitness = -std::log10(std::stod(token)); //E-Value
	return fitness;
}

double parameter::get_fitness_from_file(const string &fp, uint id) {
	auto last_slash_pos = fp.find_last_of('/');
	string new_fp = fp.substr(0, last_slash_pos) + std::to_string(id) + fp.substr(last_slash_pos);

	std::ifstream stream(new_fp);
	if(!stream) {
		cout << "Not able to open " << new_fp << '\n';
		exit(1);
	}

	string str, token;
	std::stringstream ss;

	getline(stream, str);
	getline(stream, str);
	ss = std::stringstream(str);
	for(uint i = 0; i < 5; i++) {
		ss >> token;
	}
	fitness = -std::log10(std::stod(token)); //E-Value
	return fitness;
}
