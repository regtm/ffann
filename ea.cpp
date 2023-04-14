#include <cstring>
#include <random>
#include <algorithm>
#include <iostream>
#include "ea.hpp"
#include "fitness.hpp"
#include "activation.hpp"
#include "ann.hpp"

Genome Evolution::mutate_input_weights(Genome g){
    int len = g.input_weights.size();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> int_dis(0, len);
    std::uniform_real_distribution<> double_dis(-ea_settings.mutation_range, ea_settings.mutation_range);

    for(int i = 0; i < len; i++){
        if((double) int_dis(gen) < (double) len * ea_settings.mutation_probability){
            g.input_weights[i] += double_dis(gen);
        }
    }

    return g;
}

Genome Evolution::mutate_hidden_weights(Genome g){
    int len = g.hidden_weights.size();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> int_dis(0, len);
    std::uniform_real_distribution<> double_dis(-ea_settings.mutation_range, ea_settings.mutation_range);

    for(int i = 0; i < len; i++){
        if((double) int_dis(gen) < (double) len * ea_settings.mutation_probability){
            g.hidden_weights[i] += double_dis(gen);
        }
    }

    return g;
}

Genome Evolution::recombine_input_weights(Genome g1, Genome g2){
    int split = g1.input_weights.size() / 2;

    std::memcpy(g1.input_weights.data(), g2.input_weights.data(), split * sizeof(double));

    return g1;
}

Genome Evolution::recombine_hidden_weights(Genome g1, Genome g2){
    int split = g1.hidden_weights.size() / 2;

    std::memcpy(g1.hidden_weights.data(), g2.hidden_weights.data(), split * sizeof(double));

    return g1;
}


Evolution::Evolution(
    Ea_settings ea_settings,
    Ann_settings ann_settings,
    std::function<double(Network, std::vector<data_point>)> fitness_function
    ){
        this->best_fitness = 0.0;
        this->fitness_function = fitness_function;
        this->ea_settings = ea_settings;

        this->ann_settings = ann_settings;

        //setup population
        this->population.resize(ea_settings.number_of_individuals);
}

std::tuple<Network, Genome> Evolution::eval(std::vector<data_point> set){
    
    /* Setup ANN */
    Network net = Network(ann_settings);

    net.init_network();

    /* Initialize genome weights */
    for(int i=0; i<this->population.size(); i++){
        net.init_weights();
        this->population[i].input_weights.resize(net.get_input_weights().size());
        this->population[i].hidden_weights.resize(net.get_hidden_weights().size());
        this->population[i].input_weights = net.get_input_weights();
        this->population[i].hidden_weights = net.get_hidden_weights();
    }

    int epoch = 0;

    while(best_fitness < ea_settings.stop_at_fitness && epoch < ea_settings.number_of_epochs){
        
        // Sort by fitness
        std::sort(this->population.begin(), this->population.end(), [](Genome g1, Genome g2){
            return g1.fitness > g2.fitness;
            }
        );

        std::cout << "Epoch: " << epoch << std::endl;

        // Recombine
        std::vector<Genome> new_population(this->population.size());

        for(int i=0; i < new_population.size(); i++){
            if(i < ea_settings.number_of_recombined){
                new_population[i] = recombine_input_weights(this->population[i*2], this->population[i*2+1]);
                new_population[i] = recombine_hidden_weights(this->population[i*2], this->population[i*2+1]);
            }else{
                new_population[i] = this->population[i - ea_settings.number_of_recombined];
            }
        }
        // Replace old population
        this->population = new_population;


        // Mutate genomes and evaluate net
        for(int i=0; i<this->population.size(); i++){
            this->population[i] = mutate_input_weights(this->population[i]);
            this->population[i] = mutate_hidden_weights(this->population[i]);

            net.set_input_weights(this->population[i].input_weights);
            net.set_hidden_weights(this->population[i].hidden_weights);

            //calc fitness
            double fitness_val = this->fitness_function(net, set);

            // std::cout << "Fitness: " << fitness_val << "\n";
            if(fitness_val > best_fitness){
                best_fitness = fitness_val;
                std::cout << "Best fitness: " << fitness_val;
                std::cout << " Inputw: ";
                for(auto e: this->population[i].input_weights){
                    std::cout << e << " ";
                }
                std::cout << " Hiddenw: ";
                for(auto e: this->population[i].hidden_weights){
                    std::cout << e << " ";
                }
                std::cout << std::endl;
            }
            this->population[i].fitness = fitness_val;
        }
    
        epoch++;
    }

    // Sort by fitness a final time to return the best individual
        std::sort(this->population.begin(), this->population.end(), [](Genome g1, Genome g2){
        return g1.fitness > g2.fitness;
        }
    );

    return std::tuple<Network, Genome>(net, this->population[0]);

}
