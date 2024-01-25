#include "tetris.h"
#include <random>

int Pieces[7][4][4] = {
    {{0,1,0,0},
     {0,1,0,0},
     {0,1,0,0},
     {0,1,0,0}},
    {{0,1,0,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    {{0,1,0,0},
     {0,1,0,0},
     {1,1,0,0},
     {0,0,0,0}},
    {{0,1,1,0},
     {1,1,0,0},
     {0,0,0,0},
     {0,0,0,0}},
    {{1,1,0,0},
     {1,1,0,0},
     {0,0,0,0},
     {0,0,0,0}},
    {{0,1,0,0},
     {0,1,0,0},
     {0,1,1,0},
     {0,0,0,0}},
    {{1,1,0,0},
     {0,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
};

basic::Vector2i PieceRotations[2][4][4] = {
    {{{ 2, 0},{ 1, 1},{ 0, 2},{ 0, 0}},
     {{ 1,-1},{ 0, 0},{-1, 1},{ 0, 0}},
     {{ 0,-2},{-1,-1},{-2, 0},{ 0, 0}},
     {{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}}},
    {{{ 0, 0},{-1, 1},{ 0, 0},{ 0, 0}},
     {{ 1,-1},{ 0, 0},{-1, 1},{-2, 2}},
     {{ 0, 0},{ 1,-1},{ 0, 0},{ 0, 0}},
     {{ 0, 0},{ 2,-2},{ 0, 0},{ 0, 0}}},
};

void Square::getTexture(basic::texture* t) {
    t->imageCoords = { type*8,0,8,8 };
}

void Tetris::generatePiece()
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            Square* current_square = &currentPiece[y][x];
            if (Pieces[currentPieceType][y][x] == 0)
            {
                current_square->empty = true;
                continue;
            }

            current_square->empty = false;
            current_square->type = currentPieceType;
        }
    }
}

bool Tetris::isPieceColliding()
{
    bool colliding = false;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            Square* current_pieceSquare = &currentPiece[y][x];
            Square* current_gridSquare = &grid[y + piecePosition.y][x + piecePosition.x];

            if (!current_gridSquare->empty && !current_pieceSquare->empty)
            {
                colliding = true;
                y = 4;
                break;
            }
        }
    }
    return colliding;
}

void Tetris::rotatePiece()
{
    int rotationAlgorithm = 0;

    if (currentPieceType == PieceType::O)
        return;

    if (currentPieceType == PieceType::I)
        rotationAlgorithm = 1;

    Square rotatedPiece[4][4];
    Square copyPiece[4][4];

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            Square* current_square = &currentPiece[y][x];
            if (current_square->empty)
                continue;

            Square* rotated_square = &rotatedPiece[y + PieceRotations[rotationAlgorithm][y][x].y][x + PieceRotations[rotationAlgorithm][y][x].x];
            rotated_square->empty = false;
            rotated_square->type = currentPieceType;
        }
    }

    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            copyPiece[y][x] = currentPiece[y][x];
            currentPiece[y][x] = rotatedPiece[y][x];
        }
    }

    if (isPieceColliding())
    {
        for (int y = 0; y < 4; y++)
        {
            for (int x = 0; x < 4; x++)
            {
                currentPiece[y][x] = copyPiece[y][x];
            }
        }
    }
}

void Tetris::bringDownRowsAbove(int row)
{
    for (int y = row; y > 0; y--)
    {
        for (int x = 0; x < 10; x++)
        {
            grid[y][x].type = grid[y - 1][x].type;
            grid[y][x].empty = grid[y - 1][x].empty;
        }
    }
}

void Tetris::resetGame(int randomNumber) {
    for (int y = 0; y < 20; y++) {
        for (int x = 0; x < 10; x++) {
            grid[y][x].empty = true;
        }
    }

    piecePosition = { 4,0 };
    randomPiece = randomNumber;
    currentPieceType = (PieceType)(randomPiece % 7);
    generatePiece();

    gameOver = false;
    score = 0;
}

void Tetris::checkForRows()
{
    int rowsInARow = 0;
    for (int y = 0; y < 20; y++)
    {
        int row = 0;
        for (int x = 0; x < 10; x++)
        {
            if (!grid[y][x].empty)
                row++;
        }
        if (row == 10)
        {
            bringDownRowsAbove(y);
            rowsInARow += 1;
            y--;
        }
    }
    score += (rowsInARow * rowsInARow) * 150;
}

void Tetris::addFitness(NeuralNetwork* n) {
    // Bumpiness fitness
    std::vector <int> highest;
    for (int x = 0; x < 10; x++) {
        highest.push_back(20);
        for (int y = 19; y >= 0; y--) {
            if (!grid[y][x].empty) {
                highest[x] = y;
            }
        }
    }

    float bumpiness = 0;
    for (int i = 1; i < highest.size(); i++) {
        bumpiness += std::abs(highest[i] - highest[i - 1]);
    }

    n->fitness -= bumpiness * 0.5;

    for (int y = 0; y < 4; y++) {
        bool row = true;
        for (int x = 0; x < 4; x++) {
            if (currentPiece[y][x].empty)
                continue;

            // Holes fitness
            int holes_amount = 0;
            for (int y_grid = y + piecePosition.y + 1; y_grid < 20; y_grid++) {
                if (!grid[y_grid][x + piecePosition.x].empty) {
                    n->fitness -= holes_amount * 0.5;
                    y_grid = 20;
                }
                else {
                    holes_amount++;
                }
            }

            // Creating rows fitness
            if (row) {
                row = false;
                int rowTotal = 0;

                for (int x_grid = 0; x_grid < 10; x_grid++) {
                    if (!grid[y + piecePosition.y][x_grid].empty) {
                        rowTotal++;
                    }
                    else {
                        n->fitness += rowTotal * rowTotal * ((y + piecePosition.y) / 19) * 1;
                        rowTotal = 0;
                    }
                }
            }
        }
    }
}

void Tetris::layDownPiece(NeuralNetwork* n)
{
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            if (currentPiece[y][x].empty)
                continue;

            grid[y + piecePosition.y][x + piecePosition.x].empty = false;
            grid[y + piecePosition.y][x + piecePosition.x].type = currentPieceType;
        }
    }

    addFitness(n);

    piecePosition = { 4,0 };

    randomPiece++;
    currentPieceType = (PieceType)(randomPiece % 7);
    generatePiece();

    score += 5;

    checkForRows();

    if (isPieceColliding())
        gameOver = true;
}

void Tetris::update(NeuralNetwork* n) {
    piecePosition.y += 1;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            Square* current_square = &currentPiece[y][x];
            if (current_square->empty)
                continue;

            if (y + piecePosition.y > 19)
            {
                piecePosition.y -= 1;
                layDownPiece(n);
                return;
            }
        }
    }
    if (isPieceColliding())
    {
        piecePosition.y -= 1;
        layDownPiece(n);
        return;
    }

    // Get piece collision
    piecePosition.x += pieceVelocity.x;
    if (isPieceColliding())
    {
        piecePosition.x -= pieceVelocity.x;
    }

    piecePosition.y += pieceVelocity.y;
    if (isPieceColliding())
    {
        piecePosition.y -= pieceVelocity.y;
        layDownPiece(n);
        return;
    }

    if (rotate)
    {
        rotatePiece();
    }

    // Make piece in bounds
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            Square* current_square = &currentPiece[y][x];
            if (current_square->empty)
                continue;

            if (x + piecePosition.x > 9)
            {
                piecePosition.x -= 1;
            }

            if (x + piecePosition.x < 0)
            {
                piecePosition.x += 1;
            }

            if (y + piecePosition.y > 19)
            {
                piecePosition.y -= 1;
                layDownPiece(n);
                return;
            }
        }
    }
}
