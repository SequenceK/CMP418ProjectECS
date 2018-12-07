#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <omp.h>

#include "ecs.hpp"

using namespace std;


int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  SDL_Window * window;
  window = SDL_CreateWindow("Test",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            250,
                            250,
                            SDL_WINDOW_SHOWN
                            );
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Surface * surf = IMG_Load("../data/aus.png");
  if(!surf)
    cerr << "Image not loaded\n";
  //SDL_SetColorKey(surf, SDL_False, )
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);

  SDL_Rect srcr;
  srcr.x = 0;
  srcr.y = 0;
  srcr.w = 30;
  srcr.h = 10;

  SDL_Rect dstr;
  dstr.x = 20;
  dstr.y = 20;
  dstr.w = 100;
  dstr.h = 100;

  State global{};

  bool running = true;
  SDL_Event e;
  while(running) {
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT)
        running = false;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &srcr, NULL);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, 10, 10);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
