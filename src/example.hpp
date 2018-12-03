#pragma once

#include "ecs.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Vec2f {
    float x=0, y=0;
};

struct Example {
    SDL_Window * window;
    SDL_Renderer * renderer;

    State * state;
    Component<SDL_Point> * points;
    Component<Vec2f> * vel;
    Component<Vec2f> * acc;

    Example();
    ~Example();

    void init();
    void update();
};