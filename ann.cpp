/* Fully connected feedforward network*/

#include <vector>
#include <random>
#include <functional>
#include <cstring>

struct Settings{
    int number_of_inputs;
    int number_of_hidden_neurons;
    int number_of_outputs;
};

class Network{
    private:
        Settings settings;
        std::function<double(double)> activation_function;

    public:
        Network(Settings settings, std::function<double(double)>);
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

Network::Network(Settings settings, std::function<double(double)> activation_function)
{
    this->settings = settings;

    this->activation_function = activation_function;

    total_number_of_input_weights = (settings.number_of_inputs+1) * (settings.number_of_hidden_neurons);
    total_number_of_hidden_weights = (settings.number_of_hidden_neurons+1) * (settings.number_of_outputs);
}

void Network::init_network()
{

    /* Initialize vectors with bias*/
    input_neurons = std::vector<double>(settings.number_of_inputs+1);
    hidden_neurons = std::vector<double>(settings.number_of_hidden_neurons+1, 0.0);
    output_neurons = std::vector<double>(settings.number_of_outputs, 0.0);

    input_weights = std::vector<double>(total_number_of_input_weights, 0.0);
    hidden_weights = std::vector<double>(total_number_of_hidden_weights, 0.0);

    /* bias value */
    input_neurons.back() = -1.0;
    hidden_neurons.back() = -1.0;
}

void Network::init_weights()
{
    //copied form cpp reference no idea how it realy works
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-5.0, 5.0);

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
    for(int hidden=0; hidden < settings.number_of_hidden_neurons; hidden++){
        // include bias
        for(int input=0; input < (settings.number_of_inputs + 1); input++){ 

            int weight = hidden * (settings.number_of_inputs+1) + input;
            hidden_neurons[hidden] += input_neurons[input] * input_weights[weight];
        }

        /* Activate */
        hidden_neurons[hidden] = activation_function(hidden_neurons[hidden]);
    }

    /* sum hidden layer */
    for(int output=0; output < settings.number_of_outputs; output++){
        //include bias
        for(int hidden=0; hidden < (settings.number_of_hidden_neurons + 1); hidden++){
            
            int weight = output * (settings.number_of_hidden_neurons + 1) + hidden;
            output_neurons[output] += hidden_neurons[hidden] * hidden_weights[weight];
        }
    
        /* Activate */
        output_neurons[output] = activation_function(output_neurons[output]);
    }

    return output_neurons;
}

std::vector<double> Network::get_input_weights(){
    return input_weights;
}

std::vector<double> Network::get_hidden_weights(){
    return hidden_weights;
}

void Network::set_input_weights(std::vector<double> input_weights){
    input_weights = input_weights;
}

void Network::set_hidden_weights(std::vector<double> hidden_weights){
    hidden_weights = hidden_weights;
}