CPPC = g++ -std=c++17
CFLAGS = -Wall

all: task_xor task_classification_data1 task_classification_data2 task_simulate_robot

ea: utils simulation ea.cpp fitness.cpp activation.cpp ann.cpp
	$(CPPC) -c ea.cpp simulation.cpp fitness.cpp activation.cpp ann.cpp

utils: utils.cpp
	$(CPPC) -c utils.cpp

simulation: simulation.cpp
	$(CPPC) -c simulation.cpp

task_xor: ea task_xor.cpp
	$(CPPC) -o task_xor task_xor.cpp ea.o fitness.o activation.o ann.o utils.o simulation.o

task_classification_data1: ea task_classification_data1.cpp
	$(CPPC) -o task_classification_data1 task_classification_data1.cpp ea.o fitness.o activation.o ann.o utils.o simulation.o
	
task_classification_data2: ea task_classification_data2.cpp
	$(CPPC) -o task_classification_data2 task_classification_data2.cpp ea.o fitness.o activation.o ann.o utils.o simulation.o

task_simulate_robot: ea simulation task_simulate_robot.cpp
	$(CPPC) -o task_simulate_robot task_simulate_robot.cpp simulation.o ea.o fitness.o activation.o ann.o utils.o
	
clean:
	rm task_simulate_robot task_classification_data1 task_classification_data2 task_xor utils.o ea.o fitness.o activation.o ann.o simulation.o
