#include "window.hpp"

bool allRunning = true;
void window::input(Bot* b) {
    b->tetrisGame.pieceVelocity = {0,0};
    b->tetrisGame.rotate = false;
    b->update();

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_WINDOWEVENT) {
            switch (e.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                allRunning = false;
                break;
            }
        }
        if (allRunning == false)
            running = false;
        if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                b->tetrisGame.rotate = true;
                break;
            case SDLK_s:
                b->tetrisGame.pieceVelocity.y = 1;
                break;
            case SDLK_a:
                b->tetrisGame.pieceVelocity.x = -1;
                break;
            case SDLK_d:
                b->tetrisGame.pieceVelocity.x = 1;
                break;
            }
        }
    }

    int move = b->neuralNetwork.indexOfHighestOutput();
    if (move == 0)
        b->tetrisGame.rotate = true;
    else if (move == 1)
        b->tetrisGame.pieceVelocity.y = 1;
    else if (move == 2)
        b->tetrisGame.pieceVelocity.x = -1;
    else if (move == 3)
        b->tetrisGame.pieceVelocity.x = 1;
}

void window::update(Bot* b) {
    b->tetrisGame.update(&b->neuralNetwork);
}
void window::render(Bot* b) {
    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Render falling piece
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            Square* current_square = &b->tetrisGame.currentPiece[y][x];
            if (current_square->empty)
                continue;

            current_square->getTexture(&b->texture);

            b->texture.screenCoords = { (x + b->tetrisGame.piecePosition.x) * 16, (y + b->tetrisGame.piecePosition.y) * 16, 16, 16 };
            b->texture.drawTexture(renderer);
        }
    }

    // Render map
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            Square *current_square = &b->tetrisGame.grid[y][x];
            if (current_square->empty)
                continue;
    
            current_square->getTexture(&b->texture);
    
            b->texture.screenCoords = { x*16,y*16,16,16 };
            b->texture.drawTexture(renderer);
        }
    }

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);
}
void window::setup(Bot* b) {
    SDL_Init(SDL_INIT_EVERYTHING);
    if (IMG_Init(IMG_INIT_PNG) == -1) {
        std::cout << "SDL_image has failed to load!";
    }
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf has failed to load!";
    }

    b->tetrisGame.resetGame(0);

    b->texture.loadMedia(renderer, "C:/Users/Kurt/source/repos/MachineLearningTetris/MachineLearningTetris/TetrisPieces.png");
}
void window::clean(Bot* b) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_DestroyTexture(b->texture.gTexture);

    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}
