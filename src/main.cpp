#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <omp.h>
#include <chrono>

#include "ecs.hpp"
#include "example.hpp"

using namespace std;
using namespace std::chrono;

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

  State state{};
  Component<Vec2f> pos(&state);
  Component<Vec2f> vel(&state);
  Component<Vec2f> acl(&state);
  Component<SDL_Point> points(&state);

  VelSys velsys(&pos, &vel, &state);

  Entity e1(&state);
  Vec2f * p1 = pos.create(&e1);
  Vec2f * v1 = vel.create(&e1);
  v1->x = 20;
  v1->y = 20;
  velsys.registerEntity(&e1);


  bool running = true;
  SDL_Event e;

  auto currTime = high_resolution_clock::now();
  auto prevTime = currTime;
  while(running) {
    currTime = high_resolution_clock::now();
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT)
        running = false;
    }
    velsys.update();
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &srcr, NULL);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, p1->x, p1->y);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
    auto elapsed = currTime - prevTime;
    state.dt = duration<double>(currTime-prevTime).count();
    prevTime = currTime;
    cout << state.dt << endl;
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
