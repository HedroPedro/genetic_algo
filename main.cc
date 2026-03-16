#include "main.h"

int main(int argc, char *argv[]) {
    std::srand(time(NULL));
    configuration config(argc, argv);
    population pop(5, config);
    auto param = pop.find_best(5);
    const char *input_path = config.get_input_file_path();
    const char *macs_dir = config.get_macs_dir();
    std::cout << param.get_exec_str(input_path, macs_dir) << '\n';
    return 0;
}
