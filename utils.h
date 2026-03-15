#ifndef UTILS_H_
#define UTILS_H_
#include <random>
#include <cmath>
#include <iostream>
#define BASE_DIR "to_use"
#define TOMTOM_DIR "tomtom_out"
#define MEME_DIR "meme_out"
#define MAC3_DIR "res"
using uint = unsigned int;
using std::rand;
uint get_random(uint n);
uint get_random(uint min, uint max);
double get_random();
double get_random(double min, double max);
#endif