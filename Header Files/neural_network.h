#ifndef NEURALNET
#define NEURALNET

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <fstream>

struct Weight
{
    std::vector <float> weights;
    float bias = 0;
    float activation = 0;
    Weight(int weight_amount, int bias_value)
    {
        bias = bias_value;
        for (int i = 0; i < weight_amount; i++)
        {
            weights.push_back(0);
        }
    }
    Weight() {}
};

class NeuralNetwork
{
private:
    std::vector < std::vector< Weight>> neuralNet;
public:
    std::vector < float > output;

    int fitness = 0;
    int indexOfHighestOutput();
    std::vector < std::vector< Weight>> getNeuralNet();
    void save();
    void load();
    void setNeuralNet(std::vector < std::vector< Weight>> neuralNet);
    void setInput(std::vector<float>);
    void addToNeuralNet(std::vector <int> weight_array);
    void addToOutput(int amount_of_outputs);
    void trainNeuralNet(double learnRate);
    void runPrediction();
    void printNeuralNetActivations();
    void printNeuralNet();
    void printOutputs();
};

#endif
