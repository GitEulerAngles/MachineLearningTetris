#include "bot.h"
#include "window.hpp"
#include "basic.h"
#include <thread>
#include <iostream>

#undef main

void main() {
    Bot b[AMOUNT];
    window* w[AMOUNT];

    std::cout << "Loading Setup!\n";
    for (int i = 0; i < AMOUNT; i++) {
        b[i].setup();
        w[i] = new window("Game", (i % 9) * 160, floor(i / 9) * 320 + 32, 160, 320, 20);
        w[i]->setup(&b[i]);
    }

    b[0].neuralNetwork.load();
    std::cout << "Setup Complete!\n";

    float frameDelay = 1000 / w[0]->FPS;
    float frameTime;

    while (w[0]->running) {
        for (int i = 0; i < AMOUNT; i++) {
            if (b[i].tetrisGame.gameOver)
                continue;

            w[i]->input(&b[i]);
            w[i]->update(&b[i]);
            w[i]->render(&b[i]);
        }

        bool allGameOver = true;
        for (int i = 0; i < AMOUNT; i++)
            allGameOver = allGameOver && b[i].tetrisGame.gameOver;

        if (allGameOver) {
            trainBots(b);
        }
    }

    for (int i = 0; i < AMOUNT; i++)
        w[i]->clean(&b[i]);
}
