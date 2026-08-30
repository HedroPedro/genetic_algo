#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#include "utils.h"
#include "checkpoint.h"

constexpr auto CHECKPOINT_EXTENSION = ".chk";
constexpr auto EXPERIMENT_ATTR = "EXPERIMENT";
constexpr auto SHELL_ATTR = "SHELL";
constexpr auto TOMTOM_RES_ATTR = "TSV";
constexpr auto OTHER_PARAMS_ATTR = "EXTRA";
constexpr auto MACS_DIR_ATTR = "MACS_DIR";
constexpr auto MODE_ATTR = "MODE";
constexpr auto MODE_GENETIC_ATTR = "GENETIC";
constexpr auto MODE_RANDOM_ATTR = "RANDOM";
constexpr auto POP_ATTR = "POP";
constexpr auto ITER_ATTR = "ITER";
constexpr auto BUDGET_ATTR = "BUDGET";
constexpr auto CHECKPOINT_NAME_ATTR = "CHECKPOINT";

static constexpr auto default_macs_dir = "res";
static constexpr auto default_checkpoint_name = "checkpoint.chk";

class configuration {
private:
	string _experiment;
	string _sh_path;
	string _result_path;
	string _macs_dir = default_macs_dir;
	string _other_params = "";
	string _ini_file_name;
	string _checkpoint_name = default_checkpoint_name;
	uint16_t _generations = 100;
	uint16_t _population = 100;
	uint16_t _budget = 0;
	checkpoint _chck;
	bool _rnd_is_set = false;
public:
	configuration() {};
	
	inline void sh_path(string &sh_path) {_sh_path = sh_path;} //Write only
	string sh_exec_cmd();
	string sh_exec_cmd(int id);

	inline void   experiment(string &experiment) {_experiment = experiment;}
	inline string experiment(void) const {return _experiment;}
	
	inline void   result_path(string &result_path) {_result_path = result_path;}
	inline string result_path(void) const {return _result_path;}

	inline void   macs_dir(string &macs_dir) {_macs_dir = macs_dir;}
	inline string macs_dir(void) const {return _macs_dir;}

	inline void   other_params(string &other_params) {_other_params = other_params;}
	inline string other_params(void) const {return _other_params;}

	inline void   ini_path(string &ini_path) {_ini_file_name = ini_path;}
	inline string ini_path(void) const {return _ini_file_name;}

	inline void       checkpoint_name(string &checkpoint_name) {_checkpoint_name = checkpoint_name;}
	inline string     checkpoint_name(void) const {return _checkpoint_name;}

	inline void     generations(uint16_t generations) {_generations = generations;}
	inline uint16_t generations(void) const {return _generations;}

	inline void     population(uint16_t population) {_population = population;}
	inline uint16_t population(void) const {return _population;}

	inline void     budget(uint16_t budget) {_budget = budget;}
	inline uint16_t budget(void) const {return _budget;}
	
	inline checkpoint &chck(void) {return _chck;} //Read-only

	inline void set_true_rnd_is_set() {_rnd_is_set = true;}
	inline bool rnd_is_set() const {return _rnd_is_set;}
};

#endif