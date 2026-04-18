#ifndef WORKER_H_
#define WORKER_H_
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdatomic.h>
#include "parameter.h"
constexpr size_t STACK_SIZE = (1024*1024);

enum work_types : u_char {
    GENERATE_PARAMS = 0x01,
    RUN_PARAMS = 0x7F,
    BREED_POP = 0xFF
};

struct _worker_info {
    pid_t parent, self;
    uint best_index;
    u_char *type_of_work;
};

int worker_func(void *args);

#endif