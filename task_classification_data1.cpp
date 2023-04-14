#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "ea.hpp"
#include "activation.hpp"
#include "fitness.hpp"
#include "utils.hpp"

int main(){
    // individuals, epochs, number of recombined individuals, mutation range, mutation probability, abort fitness
    Ea_settings ea_settings {2000, 100, 1000, 2.0, 0.2, 1.0};

    // input neurons, hidden neurons, output neurons, initial weight range
    // if output neurons is set to 0 the hidden neurons become the output neurons
    Ann_settings ann_settings {2, 1, 0, 2.0, Sigmoid};

    std::string file_descriptor = "data";
    std::vector<struct data_point> set = parse_data_file(file_descriptor);

    Evolution evo = Evolution(ea_settings, ann_settings, fitness_classification);

    std::tuple<Network, Genome> winner = evo.eval(set);

    Network winner_net = get<0>(winner);
    Genome winner_genome = get<1>(winner);

    winner_net.set_input_weights(winner_genome.input_weights);
    winner_net.set_hidden_weights(winner_genome.hidden_weights);

    FILE * fp = fopen("classification_results_data1", "w");

    std::cout << "Best classification: " << winner_genome.fitness << std::endl;
    std::cout << "X Y Classification Prediction" << std::endl;

    for(auto e: set){
        int output = (int) std::round(winner_net.evaluate(e.input)[0]);
        fprintf(fp, "%f %f %i %i \n", e.input[0], e.input[1], e.classification, output);
        std::cout << e.input[0] << " " << e.input[1] << " " << e.classification << " " << output << std::endl; 
    }
}