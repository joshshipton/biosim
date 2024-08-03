#include <stdio.h>
#include <stdlib.h>
#include "neural_network.h"

// forward decleration so complier deosnt get mad
typedef struct Layer Layer;
typedef struct Neuron Neuron;
typedef struct Network Network;

typedef struct LayerTopology{
  int input_neurons;
  int output_neurons;
}LayerTopology;

// Network struct to represent the entire NN
typedef struct Network{
  Layer *layers; // Pointer to array of layers
  int num_layers;
} Network;


// Struct to represent a single layer in the network
typedef struct Layer{
  Neuron *neurons; // pointer to an array of neruons (in this layer)
  int num_neurons;
} Layer;

// a neuron has a bunch of weights and a bias
// weights determine the strength of the connection between the neurons
// Each weight corresponds to an input coming into the neuron from the previous layer
// the bias is added to the sum of the weifghts before the result is passed to the activation_function helps shift it to the left or right
typedef struct Neuron{
  float bias;  // singular bias value
  float *weights; // pointer to an array of weights
  int num_weights;
} Neuron;


// f(x) = max(0,x)
// solves a bunch of problems that I don't quite understand yet ngl
// This is called "rectified linear unit (ReLU)
float activation_function(float x){
  if(x > 0) return x;
  return 0;
}

// helper function to generate a random float between -1 and 1
float random_float(){
  return ((float)rand() / RAND_MAX) * 2-1;
}


// function to propogate input through the network
float* propagate(Network* network, float* inputs, int input_size){
  float* current_inputs = inputs;
  int current_size = input_size;

  // iterate through each layer of the network
  for(int i=0;i<network->num_layers;i++){
      Layer* layer = &network->layers[i];
      float* layer_outputs = malloc(layer->num_neurons * sizeof(float));
     // calculacet the output for each neruon in the current layer
      for (int j = 0; j < layer->num_neurons; j++){
        Neuron* neuron = &layer->neurons[j];
        float sum = neuron->bias;
        // sum the products of inputs and weights
        // the change diagram goes here, the input passes through eh neron and gets changed based on its weights
        for (int k=0; k<current_size; k++){
          sum += current_inputs[k]  * neuron->weights[k];
        }
        // apply the activation_function to the sum of the inputs and weights
        layer_outputs[j] = activation_function(sum);
      }


      // In c we have to free memory (captian obvious)
      // we dont have to free the first iteraton, these inputs are from the functions paramaters
      if (i>0) free(current_inputs);

      // update the current_inputs for the next iteration
      current_inputs = layer_outputs;
      current_size = layer->num_neurons;
  }

  // return the outputs of the final layer
  return current_inputs;
}

// Function to initialize a single neuron
void initialize_neuron(Neuron* neuron, int num_inputs) {
    neuron->bias = random_float();
    neuron->num_weights = num_inputs;
    neuron->weights = malloc(num_inputs * sizeof(float));
    for (int i = 0; i < num_inputs; i++) {
        neuron->weights[i] = random_float();
    }
}

// Function to initialize a layer
Layer initialize_layer(int num_neurons, int num_inputs) {
    Layer layer;
    layer.num_neurons = num_neurons;
    layer.neurons = malloc(num_neurons * sizeof(Neuron));
    for (int i = 0; i < num_neurons; i++) {
        initialize_neuron(&layer.neurons[i], num_inputs);
    }
    return layer;
}

// Function to initialize the entire network
Network initialize_network(LayerTopology* topologies, int num_layers) {
    Network network;
    network.num_layers = num_layers;
    network.layers = malloc(num_layers * sizeof(Layer));

    for (int i = 0; i < num_layers; i++) {
        int num_inputs = (i == 0) ? topologies[i].input_neurons : topologies[i-1].output_neurons;
        network.layers[i] = initialize_layer(topologies[i].output_neurons, num_inputs);
    }

    return network;
}


