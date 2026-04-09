#ifndef CONFIGURATION_H_
#include <string>
#include <sstream>
#include <stdexcept>

class configuration {
private:
    char *input_file_path;
    char *sh_path;
    char *result_path;
    char *macs_dir = NULL;
public:
    configuration(int argc, char *argv[]);
	std::string get_sh_exec_cmd();
	const inline char *get_input_file_path(void) {return input_file_path;}
    const inline char *get_result_path(void) {return result_path;}
    const inline char *get_macs_dir(void) {return macs_dir;}
};

#endif