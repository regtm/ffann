#include <functional>
#include <vector>
#include <cstring>
#include <random>
#include "ann.cpp"
#include "activation.cpp"
#include <algorithm>
#include <iostream>

static double x_o_r(double a, double b){
    if(a >= 1.0 && b < 1.0 || a < 1.0 && b >= 1.0){
        return 1.0;
    }else{
        return 0.0;
    }
}

struct Ea_settings{
    int number_of_individuals;
    int number_of_epochs;
    int number_of_recombined;
    double mutation_range;
    double mutation_propability;
    double stop_at_fitness;
};

struct Genome{
    double fitness;
    std::vector<double> input_weights;
    std::vector<double> hidden_weights;
};

class Evolution{
    private:
        std::function<double(double)> activation_function;

        Ea_settings ea_settings;
        Ann_settings ann_settings;

        std::vector<Genome> population;

        double best_fitness;

        Genome mutate_input_weights(Genome g);
        Genome mutate_hidden_weights(Genome g);
        Genome recombine_input_weights(Genome g1, Genome g2);
        Genome recombine_hidden_weights(Genome g1, Genome g2);
        double fitness(std::vector<double> ann_input);

    public:
        void eval();

        Evolution(
            Ea_settings ea_settings,
            Ann_settings ann_settings,
            std::function<double(double)> activation_function
            );



};

Genome Evolution::mutate_input_weights(Genome g){
    int len = g.input_weights.size();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> int_dis(0, len);
    std::uniform_real_distribution<> double_dis(-ea_settings.mutation_range, ea_settings.mutation_range);

    for(int i = 0; i < len; i++){
        if((double) int_dis(gen) < (double) len * ea_settings.mutation_propability){
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
        if((double) int_dis(gen) < (double) len * ea_settings.mutation_propability){
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
    std::function<double(double)> activation_function
    ){
        this->best_fitness = 0.0;
        this->ea_settings = ea_settings;

        this->ann_settings = ann_settings;
        this->activation_function = activation_function;

        //setup population
        this->population.resize(ea_settings.number_of_individuals);
}

void Evolution::eval(){
    
    /* Setup ANN */
    Network net = Network(ann_settings, Sigmoid);

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
        std::sort(population.begin(), population.end(), [](Genome g1, Genome g2){
            return g1.fitness > g2.fitness;
        });

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
            double sum = 0.0;
            for(int i=0; i<=1;i++){
                for(int j=0; j<=1; j++){
                std::vector<double> tmp_input {(double) i, (double) j};
//                std::cout << "Net: " << net.evaluate(tmp)[0] << "\n";
                double net_ouput = net.evaluate(tmp_input)[0];
                sum += 1.0 - std::abs(x_o_r((double) i, (double) j)- net_ouput);
                }
            }
            sum = sum * 0.25;

//            std::cout << "Fitness: " << sum << "\n";
            if(sum > best_fitness){
                best_fitness = sum;
                std::cout << "Best: " << sum << "\n";
                std::cout << "Inputw: ";
                for(auto e: this->population[i].input_weights){
                    std::cout << e << " ";
                }
                std::cout << "Hiddenw: ";
                for(auto e: this->population[i].hidden_weights){
                    std::cout << e << " ";
                }
                std::cout << "\n";
            }
            this->population[i].fitness = sum;
        }
    
        epoch++;
    }

    // get plot data
    std::cout << "Plot data \n";
    for(double x=0.0; x<=1.0; x+=0.1){
        for(double y=0.0; y<=1.0; y+=0.1){
            std::vector<double> in{x,y};
            std::cout << x << " " << y << " " << net.evaluate(in)[0] << "\n"; 
        }
        
    }

}

int main(){
    Ea_settings ea_settings {2000, 2000, 10, 2.0, 0.2, 0.99};

    Ann_settings ann_settings {2, 2, 1, 2.0};

    Evolution evo = Evolution(ea_settings, ann_settings, Sigmoid);

    evo.eval();
}