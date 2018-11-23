#include <SDL2/SDL.h>
#include <iostream>
#include <omp.h>

#include "ecs.hpp"

using namespace std;

int main(int argc, char **argv) {

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window;
    window = SDL_CreateWindow("Test",
			      SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED,
			      250,
                  250,
			      SDL_WINDOW_SHOWN
			      );
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    State global{};
    ID e1 = global.createEntity();

    bool running = true;
    SDL_Event e;
    while(running) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT)
                running = false;
        }

        SDL_RenderClear(renderer);
        
        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    return 0;
}
