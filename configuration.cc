#include "configuraion.h"

configuration::configuration(int argc, char *argv[]) {
    if (argc < 3) {
        throw "Try ./alg <shell script> <path of result file>" ;
    }
    sh_path = argv[1];
    result_path = argv[2];
}

std::string configuration::get_sh_exec_cmd(void) {
    std::ostringstream oss;
    oss << "./" << sh_path;
    return oss.str();
}