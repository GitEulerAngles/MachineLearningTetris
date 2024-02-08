#ifndef BOT
#define BOT

#include "tetris.h"
#include "neural_network.h"

#define AMOUNT 10

class Bot{
private:
public:
    float grid[20][10];
    Tetris tetrisGame;
    NeuralNetwork neuralNetwork;
    basic::texture texture;
    void generateGrid();
    void setup();
    void update();
};

void trainBots(Bot(&botArr)[AMOUNT]);

#endif //!BOT
