/* Activation functions */

#include <cmath>

static float Sigmoid(float value){

    return 2.0 / (1.0 + std::exp( -2 * value) -1);
}