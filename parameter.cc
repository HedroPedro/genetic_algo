#include "parameter.h"

void parameter::crossover(parameter &other) {
	same = false;
	max_fold = other.max_fold;
	ext_size = other.ext_size;
}

void parameter::mutate(void) {
	same = false;
	uint arg_to_change = get_random(5u);
	uint step;
	bool decrement = get_random() < 0.5;
	double delta;
	switch (arg_to_change) {
		case 0:
			delta = get_random(0.001, 0.004);
			if ((decrement && q_val != min_q_val) || q_val == max_q_val) {
				q_val = max(min_q_val, q_val - delta);
				break;
			}
			q_val = min(max_q_val, q_val+delta);
		break;
		case 1:
			step = get_random(5U, 25U);
			if ((decrement && bw != min_bw) || bw == max_bw) {
				bw = max(min_bw, bw - step);
				break;
			}
			bw = min(max_bw, bw + step);
		break;
		case 2:
			step = get_random(1U, 4U);
			if((decrement && min_fold != min_min_fold) || min_fold == max_min_fold) {
				min_fold = max(min_min_fold,  min_fold - step);			
				break;
			}
			min_fold = min(max_min_fold,  min_fold + step);
		break;
		case 3:
			step = get_random(5U, 15U);
			if((decrement && max_fold != min_max_fold) || max_fold == max_max_fold) {
				max_fold = max(min_max_fold, max_fold - step);
				break;
			}
			max_fold = min(max_max_fold, max_fold + step);
		break;
		case 4:
		default:
			step = get_random(10U, 100U);
			if((decrement && ext_size != min_ext_size) || ext_size == max_ext_size) {
				ext_size = max(min_ext_size, ext_size - step);
				break;
			}
			ext_size = min(max_ext_size, ext_size + step);
		break;
	}
}

std::string parameter::get_exec_str(const char *input_fp, const char *macs_dir) {
	std::ostringstream oss;
	oss << "macs3 callpeak -t " << input_fp;
	if(macs_dir  != NULL)
		oss << " --outdir " << macs_dir;
	oss << " -q " << q_val << " --bw " << bw << " -m " 
		<< min_fold << ' ' << max_fold << " --extsize " << ext_size << " --verbose 1";
	return oss.str();
}

double parameter::get_fitness_from_file(const char *fp) {
	std::ifstream stream(fp);
	std::string str, token;
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

