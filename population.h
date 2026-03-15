#ifndef POPULATION_H_
#define POPULATION_H_
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
#include "parameter.h"
#include "utils.h"
#include "configuraion.h"
#define BEDSLOP_CMD "bedtools slop -i " MAC3_DIR "/_summits.bed -g genome.size -b 100 > summits_extended.bed"
#define BEDTOFA_CMD "bed2fasta -o sequences.fa summits_extended.bed " BASE_DIR "/GCA_000001405.29_GRCh38.p14_genomic.fna"
#define MEME_CMD "meme sequences.fa -dna -revcomp -mod oops -nmotifs 1 -w 19"
#define TOMTOM_CMD "tomtom -oc " TOMTOM_DIR " -no-ssc -verbosity 1 -min-overlap 5 -dist pearson -thresh 0.05 " MEME_DIR "/meme.txt " BASE_DIR"/JASPAR2026.txt"
using std::vector;
using std::string;
using std::clog;
constexpr uint POP_AMOUNT = 100U;
constexpr uint GENERATIONS = 100U;

class population {
private:
    parameter *params;
    uint pop_amount;
    double crossover_chance;
    double mutation_rate;
    configuration config;
public:
    population(uint pop_amount, configuration config): params(new parameter[pop_amount]), pop_amount(pop_amount),
        crossover_chance(0.6), mutation_rate(0.05), config(config) {};
    ~population() {delete params;};
    parameter find_best(uint generations);
};

#endif