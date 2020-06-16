/* Activation functions */

#include <cmath>

static double Sigmoid(double value){

    return 2.0 / (1.0 + std::exp( -2 * value)) -1.0;
}