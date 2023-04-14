#include <vector>
#include <iostream>
#include <cmath>
#include "fitness.hpp"
#include "simulation.hpp"

double x_o_r(double a, double b){
    if(a >= 1.0 && b < 1.0 || a < 1.0 && b >= 1.0){
        return 1.0;
    }else{
        return 0.0;
    }
}
 
double fitness_xor(Network net, std::vector<data_point> set){
    //calc fitness
    double sum = 0.0;
    for(int i=0; i<=1;i++){
        for(int j=0; j<=1; j++){
            std::vector<double> tmp_input {(double) i, (double) j};
//          std::cout << "Net: " << net.evaluate(tmp)[0] << "\n";
            double net_ouput = net.evaluate(tmp_input)[0]; //single output
            sum += 1.0 - std::abs(x_o_r((double) i, (double) j)- net_ouput);
        }
    }
    
    // return fitness in range 0 to 1
    return sum * 0.25;
}

double fitness_classification(Network net, std::vector<data_point> set){

    double sum = 0.0;

    for(auto e: set){
        // std::cout << e.id << std::endl;
        int output = (int) std::round(net.evaluate(e.input)[0]); //round before casting which is just truncating!
        //std::cout << e.input[0] << "," << e.input[1] << " : " << output << std::endl;
        if(e.classification == output){ //single output
            sum += 1.0;
        }
    }

    // std::cout << "Sum: " << sum << std::endl;
    // std::cout << "Set size: " << set.size() << std::endl;

    return sum / (double) set.size();
}

double fitness_simulate_robot(Network net, std::vector<data_point> set){

    Simulation_settings settings {1000, 10000, 0.01, 0.2, 0.15, 0.25, 0.1, 0.55, 0.55, 0.2, true};
    Simulation sim = Simulation(settings);

    std::tuple<int, int> ret = sim.simulate(net);
    int traveled_distance = std::get<0>(ret);
    int collisions = std::get<1>(ret);

    if(collisions > 0){
    // if(pow(collisions, 2) > traveled_distance){
         return 0.0;
    } else {
        return (traveled_distance - pow(collisions, 2))/ pow((float)settings.map_size, 2.0);
    }
}