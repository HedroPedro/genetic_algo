#ifndef CMD_PARSER_H_
#define CMD_PARSER_H_
#include <stdexcept>
#include <string.h>
#include <unordered_set>
#include <cstdlib>
#include "utils.h"
#include "configuration.h"
#include "search.h"
#include "random_search.h"
#include "genetic.h"

using std::invalid_argument;
using std::unordered_set;
using std::stoul;

constexpr auto HELP_FLAG = "-h";

search *parse_cmd(int argc, char *argv[], configuration &config);
search *parse_config_file(char const *fp, configuration &config);
search *parse_checkpoint(char const *fp, configuration &config);

#endif