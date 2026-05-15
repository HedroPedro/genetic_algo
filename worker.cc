#include "worker.h"

static void generic(int sig) {
	(int) sig;
}

/*int worker_func(void *args) {
	_worker_info *info = (_worker_info *) args;
	uint start = info->start;
	uint last = info->slice + start;
	uint amount = info->n_workers;
	uint *n_done = info->n_done;
	_parameter *params = info->params;
	pthread_mutex_t *mutex = info->mutex;

	bool work = true;
	struct sigaction sa = {0};
	sa.sa_handler = generic;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);

	sigset_t set;
	sigfillset(&set);
	sigdelset(&set, SIGUSR1);

	while(work) {
		sigsuspend(&set);
		
		switch (*(info->type_of_work)) {
		case GENERATE_PARAMS:
			for(uint i = start; i < last; i++) {
				_parameter& param = params[i];
				new__parameter(param);
			}
			pthread_mutex_lock(mutex);
				(*n_done)++;
				if(*n_done == amount) kill(info->parent, SIGUSR2);
			pthread_mutex_unlock(mutex);
			break;
		case RUN_PARAMS:
			
			break;
		case BREED_POP:
			break;
		case END:
		default:
			work = false;
			break;
		}
	}

	return 0;
}*/