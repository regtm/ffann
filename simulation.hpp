#pragma once
#include "ann.hpp"

struct Simulation_settings{
    int map_size;
    int max_time;
    float speed;
    float turn_speed;
    float sensor_max_dist;
    float sensor_angle;
    float sensor_distance;
    float init_x;
    float init_y;
    float init_heading;
    bool random_init;
};

class Simulation{
    private:
        Simulation_settings settings;
        float torusTeleport(float val);
        float raytrace(float xStart, float yStart, float dir);
        char obstacles[1000][1000];

    public:
        Simulation(Simulation_settings settings);
        std::tuple<int,int> simulate(Network net, bool save=false, const char* path = "simulation_data");
};