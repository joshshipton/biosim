// the neural net struct, a network is just a collection of layers
pub struct Network {
    layers: Vec<Layer>,
}

pub struct LayerTopology {
    // each layer will have neurons for input and neurons for output
    pub input_neurons: usize,
    pub output_neurons: usize,
}

// and each layer is a colllection of neurons
struct Layer {
    neurons: Vec<Neuron>,
}

// a beuron has a bias and then a collection of weights, a bias is
// " Bias is like a neuron's if statement - it allows for a neuron to stay inactive (return an output of zero) unless the input is strong (high) enough. Formally we'd say that bias allows to regulate neuron's activation threshold. "
struct Neuron {
    bias: f32,
    // Each neuron has multiple weights, one for each input.
    weights: Vec<f32>,
}

// Implementation block for the Network struct.
impl Network {
    // Function to propagate input through the network.
    pub fn propagate(&self, mut inputs: Vec<f32>) -> Vec<f32> {
        // Iteratively apply the propagate function of each layer to the inputs.
        // The result of each layer's propagation is passed as input to the next layer.
        self.layers
            .iter()
            .fold(inputs, |inputs, layer| layer.propagate(inputs))
    }
    pub fn new(layers: Vec<LayerTopology>) -> Self {
        Self { layers }
    }
    pub fn random(layers: &[LayerTopology]) -> Self {
        assert!(layers.len() > 1);

        let mut built_layers = Vec::new();

        for adjacent_layers in layers.windows(2) {
            let input_size = adjacent_layers[0].neurons;
            let output_size = adjacent_layers[1].neurons;

            built_layers.push(Layer::random(input_size, output_size));
            let layers = layers
                .windows(2)
                .map(|layers| Layer::random(layers[0].neurons, layers[1].neurons))
                .collect();

            Self { LayerTopology }
        }
    }
}

// Implementation block for the Layer struct.
impl Layer {
    // Function to propagate input through a single layer.
    fn propagate(&self, inputs: Vec<f32>) -> Vec<f32> {
        // Create a new vector to hold the outputs of this layer.
        let mut outputs = Vec::with_capacity(self.neurons.len());

        // Iterate over each neuron in the layer.
        for neuron in &self.neurons {
            // Propagate the input through the neuron.
            let output = neuron.propagate(&inputs);
            // Collect the output from each neuron.
            outputs.push(output);
        }
        // Return the collected outputs from all neurons in the layer.
        outputs
    }
    pub fn random(input_size: usize, output_size: usize) -> Self {
        let neurons = (0..output_size)
            .map(|_| Neuron::random(input_size))
            .collect();

        Self { neurons }
    }
}

// Implementation block for the Neuron struct.
impl Neuron {
    // Function to propagate input through a single neuron.
    fn propagate(&self, inputs: &[f32]) -> f32 {
        // Calculate the weighted sum of inputs and weights.
        let output = inputs
            .iter()
            .zip(&self.weights)
            .map(|(input, weight)| input * weight)
            .sum::<f32>();

        // Apply the bias and use ReLU activation function (max(0.0, value)).
        // https://machinelearningmastery.com/rectified-linear-activation-function-for-deep-learning-neural-networks/
        (self.bias + output).max(0.0)
    }
    pub fn random(input_size: usize) -> Self {
        let mut rng = rand::thread_rng();

        let bias = rng.gen_range(-1.0..=1.0);

        let weights = (0..input_size).map(|_| rng.gen_range(-1.0..=1.0)).collect();

        Self { bias, weights }
    }
}
