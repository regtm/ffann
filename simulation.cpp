#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include "ann.hpp"
#include "simulation.hpp"


float Simulation::torusTeleport(float val){
	if(val*settings.map_size > (float)settings.map_size){
		return (val*settings.map_size -(float)settings.map_size)/settings.map_size;
	}else if(val*settings.map_size < 0.0){
		return (val*settings.map_size +(float)settings.map_size)/settings.map_size;
	}else{
		return val;
	}
}

float Simulation::raytrace(float xStart, float yStart, float dir) {

  float dist = 0.0;
  float x = xStart, y = yStart;

  while(1) {
    if(obstacles[(int)(x*(float)settings.map_size)][(int)(y*(float)settings.map_size)]){
      return (settings.sensor_max_dist - dist)/settings.sensor_max_dist;
    } else {
      x += 0.00015 * cos(dir);
      y += 0.00015 * sin(dir);
      dist += 0.00015;

	  x = torusTeleport(x);
	  y = torusTeleport(y);
    }
    if(dist > settings.sensor_max_dist)
      return 0.0;
  }
}

std::tuple<int,int> Simulation::simulate(Network net, bool save, const char* path){
    
    int timeStep;
    float sensorLeft, sensorMid, sensorRight;
    int collisions = 0;

    int exploration_matrix[settings.map_size][settings.map_size] {0};

    // initial position
    float x,y,heading;

    if(settings.random_init){
        //random in [0,1]
        std::srand(std::time(nullptr));
        x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        heading = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    } else {
        x = settings.init_x;
        y = settings.init_y;
        heading = settings.init_heading;
    }

    FILE * fp;

    if(save) fp = fopen(path, "w");

    for(timeStep=0; timeStep<settings.max_time; timeStep++) {

	    // sensors:
	    sensorMid = Simulation::raytrace(x,y,heading+0.0);
	    sensorLeft = Simulation::raytrace(x,y,heading+settings.sensor_angle*M_PI);
	    sensorRight = Simulation::raytrace(x,y,heading-settings.sensor_angle*M_PI);

        //printf("Sensors: %f, %f, %f \n", sensorLeft, sensorMid, sensorRight);

        std::vector<double> ret = net.evaluate(std::vector<double> {sensorLeft, sensorMid, sensorRight});

        float wheelLeft = ret[0];
        float wheelRight = ret[1];

        //printf("Left: %f Right: %f \n", wheelLeft, wheelRight);

        heading += ((wheelLeft-wheelRight) * M_PI * settings.turn_speed);
        //printf("Heading: %f", heading);

	    x += settings.speed * cos(heading);
	    y += settings.speed * sin(heading);

		x = Simulation::torusTeleport(x);
		y = Simulation::torusTeleport(y);

        //log position in exploration_matrix
        exploration_matrix[(int)(x*settings.map_size)][(int)(y*settings.map_size)] = 1;

        //count collisions
        if(obstacles[(int)(x*(float)settings.map_size)][(int)(y*(float)settings.map_size)]) collisions++;

        if(save) fprintf(fp, "%i %i\n", (int)(x*settings.map_size),(int)(y*settings.map_size));
	}


	int traveled_distance = 0;
		for(int x = 0; x < settings.map_size; x++){
			for(int y = 0; y < settings.map_size; y++){
					traveled_distance += exploration_matrix[x][y];
			}
		}

    printf("Distance %i Collisions %i\n", traveled_distance, collisions);

	return std::tuple<int, int>(traveled_distance, collisions);
}

Simulation::Simulation(Simulation_settings settings){
    this->settings = settings;
    
    int i,j;
    // hardcoded map
    for(i=0;i<1000;i++)
        for(j=0;j<1000;j++)
        obstacles[i][j]=0;

    for(i=0;i<1000;i++) {
        obstacles[0][i] = 1;
        obstacles[999][i] = 1;
        obstacles[1][i] = 1;
        obstacles[998][i] = 1;
        obstacles[2][i] = 1;
        obstacles[997][i] = 1;
    }
    for(i=0;i<1000;i++) {
        obstacles[i][0] = 1;
        obstacles[i][999] = 1;
        obstacles[i][1] = 1;
        obstacles[i][998] = 1;
        obstacles[i][2] = 1;
        obstacles[i][997] = 1;
    }

    for(i=0;i<300;i++) {
        obstacles[i][399] = 1;
        obstacles[i][398] = 1;
        obstacles[i][397] = 1;
    }
    for(i=700;i<1000;i++) {
        obstacles[i][599] = 1;
        obstacles[i][598] = 1;
        obstacles[i][597] = 1;
    }
    for(i=0;i<800;i++) {
        obstacles[500][i] = 1;
        obstacles[501][i] = 1;
        obstacles[502][i] = 1;
    }
}