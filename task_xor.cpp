#include <vector>
#include <iostream>
#include "ea.hpp"
#include "activation.hpp"
#include "fitness.hpp"

int main(){
    // individuals, epochs, number of recombined individuals, mutation range, mutation probability, abort fitness
    Ea_settings ea_settings {2000, 2000, 10, 2.0, 0.2, 0.99};

    // input neurons, hidden neurons, output neurons, initial weight range
    Ann_settings ann_settings {2, 2, 1, 2.0, Sigmoid};

    // empty set
    std::vector<double> cord = {0.0, 0.0};
    data_point dp = {0, 0, cord};
    std::vector<data_point> set = {dp};

    Evolution evo = Evolution(ea_settings, ann_settings, fitness_xor);

    std::tuple<Network, Genome> winner = evo.eval(set);

    Network winner_net = get<0>(winner);
    Genome winner_genome = get<1>(winner);

    winner_net.set_input_weights(winner_genome.input_weights);
    winner_net.set_hidden_weights(winner_genome.hidden_weights);

    FILE * fp = fopen("xor_classification", "w");


    // get plot data
    for(double x=0.0; x<=1.0; x+=0.1){
        for(double y=0.0; y<=1.0; y+=0.1){
            std::vector<double> in{x,y};
            fprintf(fp, "%f %f %f \n", x, y, winner_net.evaluate(in)[0]); 
        }
        
    }
}