#ifndef POPULATION_H_
#define POPULATION_H_
#include <cstdlib>
#include <stdatomic.h>
#include <signal.h>
#include "parameter.h"
#include "utils.h"
#include "configuration.h"
#include "worker.h"
using std::vector;
using std::string;
constexpr uint POP_AMOUNT = 100U;
constexpr uint GENERATIONS = 100U;
constexpr uint PATIENCE = 65u;

class population {
protected:
	uint pop_amount;
	double crossover_chance;
	double mutation_rate;
	configuration &config;
	uint patience;
public:
	population(uint pop_amount, configuration &config) : pop_amount(pop_amount),
		crossover_chance(0.6), mutation_rate(0.1), config(config), patience(PATIENCE){};
	virtual parameter find_best(uint generations) = 0;
};

class serial_population : public population {
private:
	parameter *params;
public:
	serial_population(uint pop_amount, configuration &config) : population(pop_amount, config) {
		params = new parameter[pop_amount];
	};

	~serial_population() {
		delete[] params;
	}

	parameter find_best(uint generations) override;
	void new_generation();
};

class paralel_population : public population {
private:
	uint n_children;
	size_t total_size;
	u_char *shmmap;
	pthread_mutex_t *mutex;
	u_char *type_of_work;
	_worker_info *workers_info;
	char *input_shmmap;
	char *macs_shmmap;
	_parameter *params;
	uint *n_done;
	u_char **worker_stacks;
	bool keep_waiting;

	void init_workers();
public:
	paralel_population(uint pop_amount, uint n_children, configuration &config) : population(pop_amount, config) {
		total_size = 0;
		this->n_children = n_children;
		worker_stacks = new u_char*[n_children];
		size_t size = 0;
		const char *input_fp = config.get_input_file_path();
		const char *macs_dir = config.get_macs_dir();
		while(input_fp[size++]);
		total_size += size;
		size = 0;
		while(macs_dir[size++]);
		total_size += size;
		total_size += sizeof(u_char)+sizeof(_worker_info)*n_children+sizeof(pthread_mutex_t)+sizeof(uint);

		shmmap = (u_char *) mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

		params = (_parameter *) mmap(NULL, sizeof(_parameter)*pop_amount, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

		mutex = (pthread_mutex_t *) shmmap;
		type_of_work = ((u_char *) mutex) + sizeof(pthread_mutex_t);
		workers_info = (_worker_info *) ((u_char *) type_of_work)+sizeof(u_char);
		input_shmmap = (char *) (((u_char *) workers_info) + (sizeof(_worker_info)*n_children));
		size = 0;
		while(input_fp[size]) {
			input_shmmap[size] = input_fp[size]; 
			size++;
		}
		size++;
		input_shmmap[size] = '\0';
		macs_shmmap = input_shmmap + size;
		size = 0;
		while(macs_dir[size]) {
			macs_shmmap[size] = macs_dir[size];
			size++;
		}
		size++;
		macs_shmmap[size] = '\0';

		n_done = (uint *) macs_shmmap + size;

		for(uint i = 0; i < n_children; i++) {
			worker_stacks[i] = (u_char *) mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_PRIVATE | MAP_STACK, -1, 0);
		}

		keep_waiting = false;
		pthread_mutex_init(mutex, NULL);

		init_workers();
	};

	~paralel_population(){
		for(uint i = 0; i < n_children; i++) {
			munmap(worker_stacks[i], STACK_SIZE);
		}
		pthread_mutex_destroy(mutex);
		munmap(shmmap, total_size);
		munmap(params, sizeof(_parameter)*pop_amount);
	}

	parameter find_best(uint generations) override;
};

#endif