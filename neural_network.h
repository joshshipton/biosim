#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Network Network;
typedef struct Layer Layer;
typedef struct Neuron Neuron;


typedef struct LayerTopology{
  int input_neurons;
  int output_neurons;
}LayerTopology;



// Function declarations
Network initialize_network(LayerTopology* topologies, int num_layers);

float* propagate(Network* network, float* inputs, int input_size);
void free_network(Network* network);

#endif
