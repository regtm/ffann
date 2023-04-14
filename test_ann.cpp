#include <vector>
#include "ann.cpp"
#include "activation.cpp"
#include <iostream>

int main(){

    Ann_settings ann_settings;
    ann_settings.number_of_inputs = 2;
    ann_settings.number_of_hidden_neurons = 2;
    ann_settings.number_of_outputs = 1;
    ann_settings.initial_weight_range = 2.0;

    std::vector<double> input1;
    input1.push_back(1.0);
    input1.push_back(0.0);

    std::vector<double> input2;
    input2.push_back(0.0);
    input2.push_back(1.0);

    std::vector<double> input3;
    input3.push_back(1.0);
    input3.push_back(1.0);

    std::vector<double> input4;
    input4.push_back(0.0);
    input4.push_back(0.0);

    Network net = Network(ann_settings, Tanh);

    net.init_network();

    net.init_weights();

    std::vector<double> output = net.evaluate(input1);

    for(std::vector<double>::iterator it = output.begin(); it != output.end(); it++){
        std::cout << * it << " ";
    }

    output = net.evaluate(input2);

    for(std::vector<double>::iterator it = output.begin(); it != output.end(); it++){
        std::cout << * it << " ";
    }

    output = net.evaluate(input3);

    for(std::vector<double>::iterator it = output.begin(); it != output.end(); it++){
        std::cout << * it << " ";
    }

    output = net.evaluate(input4);

    for(std::vector<double>::iterator it = output.begin(); it != output.end(); it++){
        std::cout << * it  << " ";
    }
}
