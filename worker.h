#ifndef WORKER_H_
#define WORKER_H_
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <cstdlib>
#include <stdatomic.h>
#include <sys/signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdatomic.h>
#include "parameter.h"

#include "parameter.h"
constexpr size_t STACK_SIZE = (1024*1024);

enum work_types {
    END = 0x00,
    GENERATE_PARAMS = 0x01,
    RUN_PARAMS = 0x7F,
    BREED_POP = 0xFF
};

struct _worker_info {
    pid_t parent;
    uint best_index, n_workers, slice, start;
    u_char *type_of_work;
    _parameter *params;
    uint *n_done;
    pthread_mutex_t *mutex;
};

int worker_func(void *args);

#endif