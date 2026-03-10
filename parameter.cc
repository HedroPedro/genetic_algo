#include "parameter.h"

void parameter::crossover(parameter& other) {
	max_fold = other.max_fold;
	ext_size = other.ext_size;
}

void parameter::mutate(void) {
	uint arg_to_change = get_random(0u, 4u);
	uint step;
	bool decrement = get_random() < 0.5;
	switch (arg_to_change) {
		case 0:
			double delta = get_random(0.001, 0.004);
			if (decrement) {
				q_val = max(min_q_val, q_val - delta);
				break;
			}
			q_val = min(max_q_val, q_val+delta);
		break;
		case 1:
			step = get_random(5U, 25U);
			if (decrement) {
				bw = max(min_bw, bw - step);
				break;
			}
			bw = min(max_bw, bw + step);
		break;
		case 2:
			step = get_random(1U, 4U);
		break;
		case 3:
			step = get_random(5U, 15U);
		break;
		case 4:
		default:
			step = get_random(10U, 100U);
		break;
	}
}