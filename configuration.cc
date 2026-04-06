#include "configuration.h"

configuration::configuration(int argc, char *argv[]) {
    if (argc < 4) {
        throw std::invalid_argument("Try ./alg <input file for macs> <shell script> <file path of file with E-value> [macs dir name]");
    }
    input_file_path = argv[1];
    sh_path = argv[2];
    result_path = argv[3];
    if(argc == 5)
        macs_dir = argv[4];
}

std::string configuration::get_sh_exec_cmd() {
    std::ostringstream oss;
    oss << "./" << sh_path;
    return oss.str();
}

std::string configuration::get_sh_exec_cmd(uint id) {
    std::ostringstream oss;
    oss << "./" << sh_path << ' ' << id;
    return oss.str();
}
