#pragma once
#include <functional>
#include <vector>

struct Ann_settings{
    int number_of_inputs;
    int number_of_hidden_neurons;
    int number_of_outputs;
    double initial_weight_range;
    std::function<double(double)> activation_function;
};

class Network{
    private:
        Ann_settings ann_settings;

    public:
        Network(Ann_settings settings);
        std::vector<double> evaluate(std::vector<double> inputs);

    private:
        std::vector<double> input_neurons;
        std::vector<double> hidden_neurons;
        std::vector<double> output_neurons;

        int total_number_of_input_weights;
        int total_number_of_hidden_weights;

        std::vector<double> input_weights;
        std::vector<double> hidden_weights;

    public:
        void init_network();
        void init_weights();
    
    public:
        std::vector<double> get_input_weights();
        std::vector<double> get_hidden_weights();
        void set_input_weights(std::vector<double> input_weights);
        void set_hidden_weights(std::vector<double> hidden_weights);
};