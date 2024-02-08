#include "bot.h"

void Bot::generateGrid() {
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            if (tetrisGame.grid[y][x].empty) {
                grid[y][x] = 0;
            }
            else {
                grid[y][x] = -1;
            }
        }
    }
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (!tetrisGame.currentPiece[y][x].empty) {
                grid[y + tetrisGame.piecePosition.y][x + tetrisGame.piecePosition.x] = 1;
            }
        }
    }
}

void trainBots(Bot(&botArr)[AMOUNT]) {
    static int globalFitness = -9999;

    std::vector <int> orderedIndex;
    for (int bot = 0; bot < AMOUNT; bot++) {
        //botArr[bot].neuralNetwork.fitness += botArr[bot].tetrisGame.score;
        orderedIndex.push_back(bot);
        for (int j = 0; j < bot; j++) {
            if (botArr[orderedIndex[bot]].neuralNetwork.fitness > botArr[orderedIndex[j]].neuralNetwork.fitness) {
                // Swap the pair
                int copy = orderedIndex[bot];
                orderedIndex[bot] = orderedIndex[j];
                orderedIndex[j] = copy;
            }
        }
    }

    if (botArr[orderedIndex[0]].neuralNetwork.fitness > globalFitness) {
        globalFitness = botArr[orderedIndex[0]].neuralNetwork.fitness;
        botArr[0].neuralNetwork.setNeuralNet(botArr[orderedIndex[0]].neuralNetwork.getNeuralNet());
        botArr[0].neuralNetwork.save();
    }

    std::cout << "Global fitness: " << globalFitness << std::endl;
    std::cout << "Highest fitness: " << botArr[orderedIndex[0]].neuralNetwork.fitness << std::endl;

    int r = std::rand() % 7;
    for (int i = 0; i < AMOUNT; i++) {
        botArr[i].tetrisGame.resetGame(0);
        botArr[i].neuralNetwork.fitness = 0;
        if (i == 0)
            continue;

        botArr[i].neuralNetwork.setNeuralNet(botArr[0].neuralNetwork.getNeuralNet());
        botArr[i].neuralNetwork.trainNeuralNet(0.01f);
    }
}

void Bot::setup() {
    generateGrid();

    std::vector <int> neuralNet = { 200,128,128,64,32 };

    neuralNetwork.addToOutput(4);
    neuralNetwork.addToNeuralNet(neuralNet);
}

void Bot::update() {
    std::vector <float> input;
    generateGrid();

    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            input.push_back(grid[y][x]);
        }
    }

    neuralNetwork.setInput(input);
    neuralNetwork.runPrediction();
}
