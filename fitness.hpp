#pragma once
#include <string>
#include "ann.hpp"
#include "utils.hpp"

double x_o_r(double a, double b);

double fitness_xor(Network net, std::vector<data_point> set);

double fitness_classification(Network net, std::vector<data_point> set);

double fitness_simulate_robot(Network net, std::vector<data_point> set);