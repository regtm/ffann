#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "ea.hpp"
#include "activation.hpp"
#include "fitness.hpp"
#include "utils.hpp"
#include "simulation.hpp"

int main(){
    
    // SETUP EA

    // individuals, epochs, number of recombined individuals, mutation range, mutation probability, abort fitness
    Ea_settings ea_settings {20, 20, 10, 2.0, 0.2, 1.0};

    // input neurons, hidden neurons, output neurons, initial weight range
    // if output neurons is set to 0 the hidden neurons become the output neurons
    Ann_settings ann_settings {3, 2, 2, 2.0, Sigmoid};

    Evolution evo = Evolution(ea_settings, ann_settings, fitness_simulate_robot);

    // empty set
    std::vector<double> cord = {0.0, 0.0};
    data_point dp = {0, 0, cord};
    std::vector<data_point> set = {dp};

    std::tuple<Network, Genome> winner = evo.eval(set);

    Network winner_net = get<0>(winner);
    Genome winner_genome = get<1>(winner);

    winner_net.set_input_weights(winner_genome.input_weights);
    winner_net.set_hidden_weights(winner_genome.hidden_weights);

    Simulation_settings settings {1000, 10000, 0.01, 0.2, 0.15, 0.25, 0.1, 0.55, 0.55, 0.2, false};
    Simulation sim = Simulation(settings);

    sim.simulate(winner_net, true, "simulation_result_data");
}