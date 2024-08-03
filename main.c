// main.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "neural_network.h"
#include "genetic.h"

// Define the structure of our neural network
#define INPUT_NEURONS 3
#define HIDDEN_NEURONS 4
#define OUTPUT_NEURONS 2
#define NUM_LAYERS 3

// Fitness function for the genetic algorithm
float fitness_function(const Chromosome* chromosome) {
    // This is a placeholder. You should implement your own fitness function
    // based on how well the neural network performs in your simulation.
    float sum = 0;
    for (size_t i = 0; i < chromosome->gene_count; i++) {
        sum += chromosome->genes[i];
    }
    return sum; // This is just an example, not a meaningful fitness function
}

int main() {
    printf("Running simulation...\n");
    srand(time(NULL)); // Initialize random seed

    // Initialize neural network
    LayerTopology topologies[NUM_LAYERS] = {
        {INPUT_NEURONS, HIDDEN_NEURONS},
        {HIDDEN_NEURONS, HIDDEN_NEURONS},
        {HIDDEN_NEURONS, OUTPUT_NEURONS}
    };
    Network network = initialize_network(topologies, NUM_LAYERS);

    // Initialize genetic algorithm
    size_t population_size = 100;
    float mutation_rate = 0.01;
    float crossover_rate = 0.7;
    GeneticAlgorithm* ga = create_genetic_algorithm(population_size, mutation_rate, crossover_rate);

    // Calculate total number of weights and biases in the network
    size_t chromosome_length = 0;
    for (int i = 0; i < NUM_LAYERS; i++) {
        chromosome_length += network.layers[i].num_neurons * (i == 0 ? INPUT_NEURONS : network.layers[i-1].num_neurons);
        chromosome_length += network.layers[i].num_neurons; // For biases
    }

    // Initialize population
    Population* population = initialize_population(ga, chromosome_length);

    // Run simulation for a number of generations
    int num_generations = 100;
    for (int gen = 0; gen < num_generations; gen++) {
        // Evaluate fitness
        evaluate_fitness(population, fitness_function);

    Individual* best = get_best_individual(population);
    printf("Generation %d: Best fitness = %f, Chromosome length = %zu\n",
           gen, best->fitness, best->chromosome.gene_count);

    // Print first few genes of best individual
    printf("First 5 genes of best individual: ");
    for (int i = 0; i < 5 && i < best->chromosome.gene_count; i++) {
        printf("%f\n ", best->chromosome.genes[i]);
    }


        // Evolve population
        evolve_population(ga, population, fitness_function);
    }

    // Get best individual from final generation
    Individual* best_overall = get_best_individual(population);
    printf("Best overall fitness: %f\n", best_overall->fitness);

    // You could use the best individual to set the weights of your neural network here

    // Clean up
    free_population(population);
    free_genetic_algorithm(ga);

    // Free neural network resources
    for (int i = 0; i < network.num_layers; i++) {
        for (int j = 0; j < network.layers[i].num_neurons; j++) {
            free(network.layers[i].neurons[j].weights);
        }
        free(network.layers[i].neurons);
    }
    free(network.layers);

    return 0;
}

