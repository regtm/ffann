#pragma once
#include <vector>
#include <functional>
#include "ann.hpp"

struct Ea_settings{
    int number_of_individuals;
    int number_of_epochs;
    int number_of_recombined;
    double mutation_range;
    double mutation_probability;
    double stop_at_fitness;
};

struct Genome{
    double fitness;
    std::vector<double> input_weights;
    std::vector<double> hidden_weights;
};

class Evolution{
    private:
        Ea_settings ea_settings;
        Ann_settings ann_settings;
        std::function<double(Network, std::vector<struct data_point>)> fitness_function;

        std::vector<Genome> population;

        double best_fitness;

        Genome mutate_input_weights(Genome g);
        Genome mutate_hidden_weights(Genome g);
        Genome recombine_input_weights(Genome g1, Genome g2);
        Genome recombine_hidden_weights(Genome g1, Genome g2);
        double fitness(std::vector<double> ann_input);

    public:
        std::tuple<Network, Genome> eval(std::vector<struct data_point> set);

        Evolution(
            Ea_settings ea_settings,
            Ann_settings ann_settings,
            std::function<double(Network, std::vector<struct data_point>)> fitness_function
            );



};

int main();