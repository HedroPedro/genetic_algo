#include "main.h"

int main(int argc, char *argv[]) {
    std::srand(time(NULL));
    population pop(5, configuration(argc, argv));
    auto param = pop.find_best(5);
    std::cout << param.get_exec_str() << '\n';
    return 0;
}
