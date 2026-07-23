#include "configuration.h"

string configuration::sh_exec_cmd() {
	ostringstream oss;
	oss << "./" << _sh_path;
	return oss.str();
}

string configuration::sh_exec_cmd(int id) {
	ostringstream oss;
	oss << "./" << _sh_path << ' ' << id;
	return oss.str();
}
