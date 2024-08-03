#include "genetic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

GeneticAlgorithm* create_genetic_algorithm(size_t population_size, float mutation_rate, float crossover_rate) {
    GeneticAlgorithm* ga = malloc(sizeof(GeneticAlgorithm));
    if (ga == NULL) {
        return NULL;
    }
    ga->population_size = population_size;
    ga->mutation_rate = mutation_rate;
    ga->crossover_rate = crossover_rate;
    return ga;
}

void free_genetic_algorithm(GeneticAlgorithm* ga) {
    free(ga);
}

Population* initialize_population(GeneticAlgorithm* ga, size_t chromosome_length) {
    Population* population = malloc(sizeof(Population));
    if (population == NULL) {
        return NULL;
    }

    population->individuals = malloc(ga->population_size * sizeof(Individual));
    if (population->individuals == NULL) {
        free(population);
        return NULL;
    }

    population->population_size = ga->population_size;

    for (size_t i = 0; i < ga->population_size; i++) {
        population->individuals[i].chromosome.genes = malloc(chromosome_length * sizeof(float));
        if (population->individuals[i].chromosome.genes == NULL) {
            // Handle memory allocation failure
            // Free previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(population->individuals[j].chromosome.genes);
            }
            free(population->individuals);
            free(population);
            return NULL;
        }

        population->individuals[i].chromosome.gene_count = chromosome_length;

        for (size_t j = 0; j < chromosome_length; j++) {
            population->individuals[i].chromosome.genes[j] = ((float)rand() / RAND_MAX) * 2 - 1;  // Random value between -1 and 1
        }

        population->individuals[i].fitness = 0.0f;  // Initialize fitness to 0
    }

    return population;
}

void evaluate_fitness(Population* population, float (*fitness_function)(const Chromosome*)) {
    for (size_t i = 0; i < population->population_size; i++) {
        population->individuals[i].fitness = fitness_function(&population->individuals[i].chromosome);
    }
}

Individual* selection(Population* population) {
    const int tournament_size = 3;  // You can adjust this value
    Individual* best = NULL;

    for (int i = 0; i < tournament_size; i++) {
        size_t random_index = rand() % population->population_size;
        Individual* contestant = &population->individuals[random_index];

        if (best == NULL || contestant->fitness > best->fitness) {
            best = contestant;
        }
    }

    return best;
}
Chromosome crossover(const Chromosome* parent1, const Chromosome* parent2) {
    size_t chromosome_length = parent1->gene_count;
    Chromosome child;
    child.genes = malloc(chromosome_length * sizeof(float));
    if (child.genes == NULL) {
        // Handle memory allocation failure
        child.gene_count = 0;
        return child;
    }
    child.gene_count = chromosome_length;

    size_t crossover_point = rand() % chromosome_length;

    for (size_t i = 0; i < chromosome_length; i++) {
        if (i < crossover_point) {
            child.genes[i] = parent1->genes[i];
        } else {
            child.genes[i] = parent2->genes[i];
        }
    }

    return child;
}


void mutate(Chromosome* chromosome, float mutation_rate) {
    for (size_t i = 0; i < chromosome->gene_count; i++) {
        if ((float)rand() / RAND_MAX < mutation_rate) {
            chromosome->genes[i] += ((float)rand() / RAND_MAX) * 0.2 - 0.1;  // Small random change
            // Ensure the gene stays within [-1, 1] range
            if (chromosome->genes[i] > 1.0) chromosome->genes[i] = 1.0;
            if (chromosome->genes[i] < -1.0) chromosome->genes[i] = -1.0;
        }
    }
}

int compare_individuals(const void* a, const void* b) {
    const Individual* ind_a = (const Individual*)a;
    const Individual* ind_b = (const Individual*)b;
    if (ind_a->fitness < ind_b->fitness) return 1;
    if (ind_a->fitness > ind_b->fitness) return -1;
    return 0;
}


void evolve_population(GeneticAlgorithm* ga, Population* population, float (*fitness_function)(const Chromosome*)) {

    // Sort the current population by fitness (descending order)
    qsort(population->individuals, population->population_size, sizeof(Individual), compare_individuals);

    Population new_population;
    new_population.population_size = population->population_size;
    new_population.individuals = malloc(new_population.population_size * sizeof(Individual));
    if (new_population.individuals == NULL) {
        // Handle memory allocation failure
        return;
    }

    // Elitism: Keep the top 10% of individuals
    size_t elite_count = population->population_size / 10;
    for (size_t i = 0; i < elite_count; i++) {
        new_population.individuals[i] = population->individuals[i];
        new_population.individuals[i].chromosome.genes = malloc(population->individuals[i].chromosome.gene_count * sizeof(float));
        if (new_population.individuals[i].chromosome.genes == NULL) {
            // Handle memory allocation failure
            // Free previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(new_population.individuals[j].chromosome.genes);
            }
            free(new_population.individuals);
            return;
        }
        memcpy(new_population.individuals[i].chromosome.genes, population->individuals[i].chromosome.genes,
               population->individuals[i].chromosome.gene_count * sizeof(float));
    }

    // Generate the rest of the new population
    for (size_t i = elite_count; i < new_population.population_size; i++) {
        Individual* parent1 = selection(population);
        Individual* parent2 = selection(population);

        new_population.individuals[i].chromosome = crossover(&parent1->chromosome, &parent2->chromosome);
        if (new_population.individuals[i].chromosome.genes == NULL) {
            // Handle memory allocation failure
            // Free previously allocated memory
            for (size_t j = 0; j < i; j++) {
                free(new_population.individuals[j].chromosome.genes);
            }
            free(new_population.individuals);
            return;
        }

        if ((float)rand() / RAND_MAX < ga->mutation_rate) {
            mutate(&new_population.individuals[i].chromosome, ga->mutation_rate);
        }
    }

    // Evaluate fitness of new population (except elites)
    for (size_t i = elite_count; i < new_population.population_size; i++) {
        new_population.individuals[i].fitness = fitness_function(&new_population.individuals[i].chromosome);
    }

    // Free old population and replace with new population
    free_population(population);
    *population = new_population;

}

Individual* get_best_individual(Population* population) {
    Individual* best = &population->individuals[0];
    for (size_t i = 1; i < population->population_size; i++) {
        if (population->individuals[i].fitness > best->fitness) {
            best = &population->individuals[i];
        }
    }
    return best;
}

void free_population(Population* population) {
    if (population == NULL) return;
    if (population->individuals != NULL) {
        for (size_t i = 0; i < population->population_size; i++) {
            free(population->individuals[i].chromosome.genes);
        }
        free(population->individuals);
    }
    free(population);
}

