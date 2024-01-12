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


impl Neuron {
    fn propagate(&self, inputs: &[f32]) -> f32 {
        let output = inputs
            .iter()
            .zip(&self.weights)
            .map(|(input, weight)| input * weight)
            .sum::<f32>();

        (self.bias + output).max(0.0)
    }
}