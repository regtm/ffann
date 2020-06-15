/* Fully connected feedforward network */

#include <vector>

struct Settings{
    int number_of_inputs = n_input;
    int number_of_hidden_neurons = n_hidden;
    int number_of_outputs = n_output;
};

class Network{
    private:
        Settings settings;

    public:
        Network(Settings settings);
        evaluate(vector<float> inputs);

    private:
        vector<float> input_neurons;
        vector<float> hidden_neurons;
        vector<float> output_neurons;

        vector<float> input_weights;
        vector<float> hidden_weights;
    
};

Network::Network(Settings settings)
{
    settings = settings;
}

Network::init_network()
{

    /* Initialize vectors with bias*/
    input_neurons = vector<float>(settings.number_of_inputs+1);
    hidden_neurons = vector<float>(settings.number_of_hidden_neurons+1);
    output_neurons = vector<float>(settings.number_of_outputs+1);

    input_weights = vector<float>((settings.number_of_inputs+1) * (settings.number_of_hidden_neurons+1));
    hidden_weights = vector<float>((settings.number_of_hidden_neurons+1) * (settings.number_of_outputs));

    input_neurons.back() = -1.0;
    hidden_neurons.back() = -1.0;
}

Network::init_weights()
{

}

Network::evaluate()
{

}