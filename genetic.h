#ifndef GENETIC_H
#define GENETIC_H

#include <stddef.h>

// Define your structures
typedef struct {
    float *genes;
    size_t gene_count;
} Chromosome;

typedef struct {
    Chromosome chromosome;
    float fitness;
} Individual;

typedef struct {
    Individual *individuals;
    size_t population_size;
} Population;

typedef struct {
    // Add any parameters for your genetic algorithm here
    size_t population_size;
    float mutation_rate;
    float crossover_rate;
} GeneticAlgorithm;

// Function prototypes

// Create and initialize a new genetic algorithm
GeneticAlgorithm* create_genetic_algorithm(size_t population_size, float mutation_rate, float crossover_rate);

// Free memory allocated for genetic algorithm
void free_genetic_algorithm(GeneticAlgorithm* ga);

// Initialize a population
Population* initialize_population(GeneticAlgorithm* ga, size_t chromosome_length);

// Evaluate fitness for each individual in the population
void evaluate_fitness(Population* population, float (*fitness_function)(const Chromosome*));

// Perform selection (e.g., tournament selection)
Individual* selection(Population* population);

// Perform crossover between two parent chromosomes
Chromosome crossover(const Chromosome* parent1, const Chromosome* parent2);

// Perform mutation on a chromosome
void mutate(Chromosome* chromosome, float mutation_rate);

// Evolve the population for one generation
void evolve_population(GeneticAlgorithm* ga, Population* population, float (*fitness_function)(const Chromosome*));

// Get the best individual from the population
Individual* get_best_individual(Population* population);

// Free memory allocated for population
void free_population(Population* population);

#endif // GENETIC_H
