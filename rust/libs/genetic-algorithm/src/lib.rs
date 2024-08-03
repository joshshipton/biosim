use rand::RngCore;
use rand::SeedableRng;
use rand_chacha::ChaCha8Rng;
use std::iter::FromIterator;


pub struct GeneticAlgorithm;

// the selection bethod takes a random number and a population of indivuals
// it then returns a single indivudal
impl SelectionMethod for RouletteWheelSelection{
    fn select<'a, I>(
       &self,
       rng: &mut dyn RngCore,
       population: &'a [I],
    ) -> &'a I
    where
        I: Individual
{
    population
    .choose_weighted(rng, |individual| individual.fitness())
    .expect("got an empty population")
}
    }


impl GeneticAlgorithm {
    pub fn new() -> Self{
        Self {}
    }
}

impl GeneticAlgorithm {
    // function to evolve a population
    pub fn evolve<I>(&self, population: &[I], evaluate_fitness: &dyn Fn(&I)) -> Vec<I>{
        // make sure its not empty
        assert!(!population.is_empty());    
        
        // get the size of the population
        (0..population.len())
        // need to do selection
        // selection needs to satisfy the following properties
        // 1. each individual should have a non-zero chance of being picked
        // 2. an individual with a higher fittness score should get picked, on average, more often than an individual with a lower fitness score.
        // https://www.tutorialspoint.com/genetic_algorithms/genetic_algorithms_parent_selection.htm
        .map(|_| {
            todo!()
        }).collect()
    }
}

#[cfg(test)]
#[derive(Clone, Debug)]
pub struct TestIndividual {
    fitness: f32,
}

#[cfg(test)]
impl TestIndividual {
    pub fn new(fitness: f32) -> Self {
        Self { fitness }
    }
}

#[cfg(test)]
impl Individual for TestIndividual {
    fn fitness(&self) -> f32 {
        self.fitness
    }
}

mod tests {
    use super::*; 
    
    #[test]
fn test() {
    let method = RouletteWheelSelection::new();
    let mut rng = ChaCha8Rng::from_seed(Default::default());

    let population = vec![
        TestIndividual::new(2.0),
        TestIndividual::new(1.0),
        TestIndividual::new(4.0),
        TestIndividual::new(3.0),
    ];

    let mut actual_histogram = BTreeMap::new();

    for _ in 0..1000 {
        let fitness = method
            .select(&mut rng, &population)
            .fitness() as i32;

        *actual_histogram
            .entry(fitness)
            .or_insert(0) += 1;
    }

    let expected_histogram = maplit::btreemap! {
        // fitness => how many times this fitness has been chosen
        1 => 98,
        2 => 202,
        3 => 278,
        4 => 422,
    };
    

    assert_eq!(actual_histogram, expected_histogram);
}
}
