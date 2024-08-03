#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

typedef struct Neuron {
    float bias;
    float *weights;
    int num_weights;
} Neuron;

typedef struct Layer {
    Neuron *neurons;
    int num_neurons;
} Layer;

typedef struct Network {
    Layer *layers;
    int num_layers;
} Network;

typedef struct LayerTopology {
    int input_neurons;
    int output_neurons;
} LayerTopology;

float activation_function(float x);
float random_float();
float* propagate(Network* network, float* inputs, int input_size);
void initialize_neuron(Neuron* neuron, int num_inputs);
Layer initialize_layer(int num_neurons, int num_inputs);
Network initialize_network(LayerTopology* topologies, int num_layers);

#endif // NEURAL_NETWORK_H
