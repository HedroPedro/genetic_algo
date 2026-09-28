#ifndef UTILS_H_
#define UTILS_H_
#include <random>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
using std::rand;
using std::vector;
using std::string;
using std::getline;
using std::cout;
using std::ostringstream;
using std::fstream;
using std::stoul;
using std::thread;

namespace rng {
    void init(std::mt19937& engine);

    std::mt19937& global();

    template <typename T>
    T get_random(T n);

    template <typename T>
    T get_random(T min, T max);
    
    double get_random();
}

#endif