#ifndef UTILS_H_
#define UTILS_H_
#include <random>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdint.h>
using std::rand;
using std::vector;
using std::string;
using std::getline;
using std::cout;
using std::ostringstream;
using std::fstream;

uint get_random(uint n);
uint get_random(uint min, uint max);
double get_random();
double get_random(double min, double max);
#endif