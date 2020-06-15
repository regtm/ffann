#include <vector>
#include "ann.cpp"
#include "activation.cpp"
#include <iostream>

int main(){

    Settings settings;
    settings.number_of_inputs = 2;
    settings.number_of_hidden_neurons = 2;
    settings.number_of_outputs =1;

    std::vector<double> input;
    input.push_back(1.0);
    input.push_back(0.0);

    Network net = Network(settings, Sigmoid);

    net.init_network();

    net.init_weights();

    std::vector<double> output = net.evaluate(input);

    for(std::vector<double>::iterator it = output.begin(); it != output.end(); it++){
        std::cout << * it;
    }
}