#ifndef CONFIGURATION_H_
#include <string>
#include <sstream>
class configuration {
private:
    char *sh_path;
    char *result_path;
public:
    configuration(int argc, char *argv[]);
    std::string get_sh_exec_cmd(void);
    const inline char *get_result_path(void) {return result_path;}
};

#endif