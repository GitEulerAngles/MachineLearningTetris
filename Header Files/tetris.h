#ifndef TETRIS
#define TETRIS

#include "basic.h"
#include "neural_network.h"

enum PieceType
{
    I = 0,
    T = 1,
    L = 2,
    Z = 3,
    O = 4,
    J = 5,
    C = 6
};

struct Square
{
    void getTexture(basic::texture* t);
    bool empty = true;
    PieceType type;
};

class Tetris
{
public:
    bool rotate;
    int score = 0;
    int fallTick = 0;
    int randomPiece = 0;
    bool gameOver = false;
    PieceType currentPieceType;
    Square currentPiece[4][4];
    basic::Vector2i piecePosition = {4,0};
    basic::Vector2i pieceVelocity = {0,0};
    Square grid[20][10];
    void generatePiece();
    void layDownPiece(NeuralNetwork* n);
    void rotatePiece();
    void resetGame(int randomNumber);
    bool isPieceColliding();
    void update(NeuralNetwork *n);
private:
    void addFitness(NeuralNetwork* n);
    void checkForRows();
    void bringDownRowsAbove(int row);
}; 

#endif
