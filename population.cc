#include "population.h"

parameter population::find_best(uint generations) {
    parameter elitist;
    elitist.empty_param();
    uint i, j;
    double fitness, random;
    bool changed;
    std::string sh_cmd = config.get_sh_exec_cmd();
    const char *input_fp = config.get_input_file_path();
    const char *res_fp = config.get_result_path();
    const char *macs_dir = config.get_macs_dir();
    std::ofstream csv("generations.csv");
    csv << "Generation;Fitness\n";
    for(i = 0; i < generations; i++) {
        changed = false;
        for(j = 0; j < pop_amount; j++) {
            parameter &cache = params[j];
            string cmd = cache.get_exec_str(input_fp, macs_dir);
            const char *cmd_c = cmd.c_str();
            if (std::system(cmd_c)) {
                std::exit(1);
            }
            if (std::system(sh_cmd.c_str())) {
                std::exit(1);
            }
            fitness = params[j].set_fitness_from_file(res_fp);
            std::cout << "Generation:" << i << ';' << "Member:" << j 
                << ';' << "Fit:" << fitness << '\n';
            if (fitness >= elitist.get_fitness()) {
                parameter::replace(elitist, cache);
                changed = true;
            }
        }

        csv << i << ';' << elitist.get_fitness() << '\n';

        if (!changed) {
            j = get_random(pop_amount);
            parameter::replace(params[j], elitist);
        }

        for(j = 0; j < 2; j++) {
            random = get_random();
            parameter &cache = params[j];
            if (random < crossover_chance) {
                cache.crossover(elitist);
            }
            random = get_random();
            if (random < mutation_rate) {
                cache.mutate();
            }
        }
    }

    csv.close();
    return elitist;
}