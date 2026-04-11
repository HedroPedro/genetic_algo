#include "main.h"

int main(int argc, char *argv[]) {
    std::srand(time(NULL));
    
    configuration config(argc, argv);
    population *pop;
    #ifdef THREADS
    thread_population t(POP_AMOUNT, config, 4);
    pop = &t;
    #else
    population p(10, config);
    pop = &p;
    #endif
    auto param = pop->find_best(GENERATIONS);
    const char *input_path = config.get_input_file_path();
    const char *macs_dir = config.get_macs_dir();
    auto cmd = param.get_exec_str(input_path, macs_dir);
    auto cmd_c = cmd.c_str();
    std::system(cmd_c);
    return 0;
}
