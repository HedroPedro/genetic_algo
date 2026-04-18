#include "main.h"

int main(int argc, char *argv[]) {
    std::srand(time(NULL));
    configuration config(argc, argv);
    population *pop;
    serial_population p(POP_AMOUNT*10, config);
    pop = &p;
    auto param = pop->find_best(GENERATIONS);
    const char *input_path = config.get_input_file_path();
    const char *macs_dir = config.get_macs_dir();
    auto cmd = param.get_exec_str(input_path, macs_dir);
    auto cmd_c = cmd.c_str();
    std::cout << cmd_c << std::endl;
    std::system(cmd_c);
    return 0;
}
