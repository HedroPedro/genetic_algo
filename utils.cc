#include "utils.h"

uint get_random(uint min, uint max) {
    int size = max - min;
    return rand() % size + min;
}

double get_random() {
    return rand() / RAND_MAX;
}

double get_random(double min, double max) {
    return get_random() * (max-min) + min;
}