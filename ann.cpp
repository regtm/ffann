/* Fully connected feedforward network*/

#include <random>
#include <cstring>
#include "ann.hpp"
#include "activation.hpp"

Network::Network(Ann_settings settings)
{
    this->ann_settings = settings;

    total_number_of_input_weights = (settings.number_of_inputs+1) * (settings.number_of_hidden_neurons);
    total_number_of_hidden_weights = (settings.number_of_hidden_neurons+1) * (settings.number_of_outputs);
}

void Network::init_network()
{

    /* Initialize vectors with bias*/
    input_neurons = std::vector<double>(ann_settings.number_of_inputs+1);
    hidden_neurons = std::vector<double>(ann_settings.number_of_hidden_neurons+1, 0.0);
    output_neurons = std::vector<double>(ann_settings.number_of_outputs, 0.0);

    input_weights = std::vector<double>(total_number_of_input_weights, 0.0);
    hidden_weights = std::vector<double>(total_number_of_hidden_weights, 0.0);

    /* bias value */
    input_neurons.back() = -1.0;
    hidden_neurons.back() = -1.0;
}

void Network::init_weights()
{
    //copied from cpp reference no idea how it realy works
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-ann_settings.initial_weight_range, ann_settings.initial_weight_range);

    /* Set random weights */
    for(int i=0; i<total_number_of_input_weights; i++){
        input_weights[i] = dis(gen);
    }

    for(int i=0; i<total_number_of_hidden_weights; i++){
        hidden_weights[i] = dis(gen);
    }
}

std::vector<double> Network::evaluate(std::vector<double> inputs)
{
    // overwrite all but the bias input
    std::memcpy(input_neurons.data(), inputs.data(), inputs.size() * sizeof(double));

    /* sum input layer */
    for(int hidden=0; hidden < ann_settings.number_of_hidden_neurons; hidden++){
        // include bias
        for(int input=0; input < (ann_settings.number_of_inputs + 1); input++){ 

            int weight = hidden * (ann_settings.number_of_inputs+1) + input;
            hidden_neurons[hidden] += input_neurons[input] * input_weights[weight];
        }

        /* Activate */
        hidden_neurons[hidden] = this->ann_settings.activation_function(hidden_neurons[hidden]);
    }

    /* sum hidden layer */
    for(int output=0; output < ann_settings.number_of_outputs; output++){
        //include bias
        for(int hidden=0; hidden < (ann_settings.number_of_hidden_neurons + 1); hidden++){
            
            int weight = output * (ann_settings.number_of_hidden_neurons + 1) + hidden;
            output_neurons[output] += hidden_neurons[hidden] * hidden_weights[weight];
        }
    
        /* Activate */
        output_neurons[output] = this->ann_settings.activation_function(output_neurons[output]);
    }

    /* Allow networks without hidden layer */
    if( ann_settings.number_of_outputs > 0){
        return output_neurons;
    } else {
        return hidden_neurons;
    }
    
}

std::vector<double> Network::get_input_weights(){
    return input_weights;
}

std::vector<double> Network::get_hidden_weights(){
    return hidden_weights;
}

void Network::set_input_weights(std::vector<double> input_weights){
    this->input_weights = input_weights;
}

void Network::set_hidden_weights(std::vector<double> hidden_weights){
    this->hidden_weights = hidden_weights;
}
