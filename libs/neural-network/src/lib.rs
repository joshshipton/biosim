pub struct Network{
    layers: Vec<Layer>
}

struct Layer
{
    neruons: Vec<Neuron>
}

struct Neuron
{
    bias: f32, 
    weivghts: Vec<f32>,
}


impl Network{
pub fn propagate(&self, mut inputs: Vec<f32>) -> Vec<f32>{
    self.layers.iter().fold(inputs, |inputs, layer| layer.propagate(inputs));  
    }
}

impl Layer{
    fn propagate(&self, inputs: Vec<f32> -> Vec<f32>)
    let mut outputs = Vec::with_capacity(self.neurons.len());


    for neuron in &self.neurons
    {
        let output = neuron.propagate(&inputs);
        outputs.push(output);
    }
    outputs; 
}


impl Neuron{
fn propagate(&self, inputs: &[f32]) -> f32 {
    assert_eq!(inputs.len(), self.weights.len());

    let mut output = 0.0;

    for i in 0..inputs.len() {
        output += inputs[i] * self.weights[i];
    }

    output += self.bias;

    if output > 0.0 {
        output
    } else {
        0.0
    }
}}