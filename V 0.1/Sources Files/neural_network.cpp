#include "neural_network.h"

void NeuralNetwork::addToNeuralNet(std::vector <int> weight_array)
{
    for (int x = 0; x < weight_array.size(); x++)
    {
        std::vector<Weight> neuralRow;
        for (int y = 0; y < weight_array[x]; y++)
        {
            Weight w;
            if (x == weight_array.size() - 1)
            {
                w = Weight(output.size(), 0);
            }
            else
            {
                w = Weight(weight_array[x + 1], 0);
            }
            neuralRow.push_back(w);
        }
        neuralNet.push_back(neuralRow);
    }
}

void NeuralNetwork::addToOutput(int amount_of_outputs)
{
    for (int i = 0; i < amount_of_outputs; i++)
    {
        output.push_back(0);
    }
}

double randomNumberFromRate(double rate)
{
    static std::random_device generator;
    static std::mt19937 engine(generator());

    std::uniform_real_distribution<> distr(-rate, rate);

    return distr(engine);
}

void NeuralNetwork::trainNeuralNet(double learnRate)
{
    for (int layer = 0; layer < neuralNet.size(); layer++)
    {
        for (int neuron = 0; neuron < neuralNet[layer].size(); neuron++)
        {
            if (layer != 0)
                neuralNet[layer][neuron].bias += randomNumberFromRate(learnRate);
            else
                neuralNet[layer][neuron].bias = 0;

            if (neuralNet[layer][neuron].bias < -5) // Cap it at zero
                neuralNet[layer][neuron].bias = -5;
            if (neuralNet[layer][neuron].bias > 5)
                neuralNet[layer][neuron].bias = 5;
            for (int weight = 0; weight < neuralNet[layer][neuron].weights.size(); weight++)
            {
                neuralNet[layer][neuron].weights[weight] += randomNumberFromRate(learnRate);
                if (neuralNet[layer][neuron].weights[weight] < -2) // Cap it at zero
                    neuralNet[layer][neuron].weights[weight] = -2;
                if (neuralNet[layer][neuron].weights[weight] > 2) // Cap it at two
                    neuralNet[layer][neuron].weights[weight] = 2;
            }
        }
    }
}

float sigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

void NeuralNetwork::runPrediction()
{
    // Reset NeuralNet
    for (int x = 1; x < neuralNet.size(); x++)
    {
        for (int y = 0; y < neuralNet[x].size(); y++)
        {
            neuralNet[x][y].activation = 0;
        }
    }
    // Reset outputs
    for (int x = 0; x < output.size(); x++)
    {
        output[x] = 0;
    }
    for (int layer = 1; layer < neuralNet.size(); layer++)
    {
        for (int neuron = 0; neuron < neuralNet[layer].size(); neuron++)
        {
            for (int weight = 0; weight < neuralNet[layer - 1].size(); weight++)
            {
                neuralNet[layer][neuron].activation += neuralNet[layer - 1][weight].weights[neuron] * neuralNet[layer - 1][weight].activation;
            }
            neuralNet[layer][neuron].activation = sigmoid(neuralNet[layer][neuron].activation + neuralNet[layer][neuron].bias);
        }
    }
    for (int neuron = 0; neuron < output.size(); neuron++)
    {
        for (int weight = 0; weight < neuralNet[neuralNet.size() - 1].size(); weight++)
        {
            output[neuron] += neuralNet[neuralNet.size() - 1][weight].weights[neuron] * neuralNet[neuralNet.size() - 1][weight].activation;
        }
        output[neuron] = sigmoid(output[neuron]);
    }
}

int NeuralNetwork::indexOfHighestOutput()
{
    int highestIndex = 0;
    for (int i = 1; i < output.size(); i++)
    {
        if (output[i] > output[highestIndex])
        {
            highestIndex = i;
        }
    }
    return highestIndex;
}

void NeuralNetwork::save() {
    std::fstream f("NN.dat", std::ios::out | std::ios::binary);

    if (!f.is_open()) {
        std::cout << "Error opening file for saving neural network" << std::endl;
        return;
    }

    size_t num_layers = neuralNet.size();
    f.write(reinterpret_cast<char*>(&num_layers), sizeof(num_layers));

    for (auto& layer : neuralNet) {
        size_t num_weights = layer.size();
        f.write(reinterpret_cast<char*>(&num_weights), sizeof(num_weights));

        for (auto& weight : layer) {
            // Save each weight's details
            size_t weights_size = weight.weights.size();
            f.write(reinterpret_cast<char*>(&weights_size), sizeof(weights_size));
            f.write(reinterpret_cast<char*>(weight.weights.data()), weights_size * sizeof(weight.weights[0]));
            f.write(reinterpret_cast<char*>(&weight.bias), sizeof(weight.bias));
        }
    }

    f.close();
}

void NeuralNetwork::load() {
    std::fstream f("NN.dat", std::ios::in | std::ios::binary);

    if (!f.is_open()) {
        std::cout << "Error opening file for loading neural network" << std::endl;
        return;
    }

    size_t num_layers = 0;
    f.read(reinterpret_cast<char*>(&num_layers), sizeof(num_layers));
    neuralNet.resize(num_layers);

    for (auto& layer : neuralNet) {
        size_t num_weights = 0;
        f.read(reinterpret_cast<char*>(&num_weights), sizeof(num_weights));
        layer.resize(num_weights);

        for (auto& weight : layer) {
            // Load each weight's details
            size_t weights_size = 0;
            f.read(reinterpret_cast<char*>(&weights_size), sizeof(weights_size));
            weight.weights.resize(weights_size);
            f.read(reinterpret_cast<char*>(weight.weights.data()), weights_size * sizeof(weight.weights[0]));
            f.read(reinterpret_cast<char*>(&weight.bias), sizeof(weight.bias));
        }
    }

    f.close();
}


void NeuralNetwork::setNeuralNet(std::vector<std::vector<Weight>> net)
{
    for (int x = 0; x < neuralNet.size(); x++)
    {
        for (int y = 0; y < neuralNet[x].size(); y++)
        {
            neuralNet[x][y].bias = net[x][y].bias;
            for (int z = 0; z < neuralNet[x][y].weights.size(); z++)
            {
                neuralNet[x][y].weights[z] = net[x][y].weights[z];
            }
        }
    }
}

std::vector<std::vector<Weight>> NeuralNetwork::getNeuralNet()
{
    return neuralNet;
}

void NeuralNetwork::setInput(std::vector<float> input)
{
    for (int i = 0; i < neuralNet[0].size(); i++)
    {
        neuralNet[0][i].activation = input[i];
    }
}

void NeuralNetwork::printNeuralNet()
{
    for (int x = 0; x < neuralNet.size(); x++)
    {
        std::cout << "Row: " << x << std::endl;
        for (int y = 0; y < neuralNet[x].size(); y++)
        {
            std::cout << neuralNet[x][y].bias << "\n\tweights: ";
            for (int z = 0; z < neuralNet[x][y].weights.size(); z++)
            {
                std::cout << neuralNet[x][y].weights[z] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void NeuralNetwork::printNeuralNetActivations()
{
    for (int x = 0; x < neuralNet.size(); x++)
    {
        std::cout << "\nLine: " << x << std::endl;
        for (int y = 0; y < neuralNet[x].size(); y++)
        {
            std::cout << neuralNet[x][y].activation << " ";
        }
    }
    std::cout << std::endl;
}

void NeuralNetwork::printOutputs()
{
    std::cout << "\nOutputs: ";
    for (int x = 0; x < output.size(); x++)
    {
        std::cout << output[x] << " ";
    }
    std::cout << std::endl;
}
