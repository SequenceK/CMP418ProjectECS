#include "example.hpp"



Example::Example() {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Test",
                  SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED,
                  250,
                  250,
                  SDL_WINDOW_SHOWN
                  );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    state = new State();

    points = new Component<SDL_Point>(state);
}
