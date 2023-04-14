#include <cmath>
#include "activation.hpp"

double Tanh(double value){

    return 2.0 / (1.0 + std::exp( -2 * value)) -1.0;
}

double Sigmoid(double value){
    return 1.0 / (1.0 + std::exp(-value));
}
