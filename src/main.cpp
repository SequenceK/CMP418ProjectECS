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
  omp_set_num_threads(8);
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
  SDL_GL_SetSwapInterval(0);

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
  SDLPointUpdate pupdate(&pos, &points, &state);

  constexpr unsigned int esize = 100000;
  Entity * entities[esize];

  for(int i = 0; i < esize; i++) {
    Entity * e  = new Entity(&state);
    entities[i] = e;
    Vec2f * p1 = pos.create(e);
    Vec2f * v1 = vel.create(e);
    SDL_Point * point = points.create(e);
    v1->x = 20;
    v1->y = 20;
    p1->y = 10*i;
    velsys.registerEntity(e);
    pupdate.registerEntity(e);
  }

  bool running = true;
  SDL_Event e;

  state.init();

  auto currTime = high_resolution_clock::now();
  auto prevTime = currTime;
  while(running) {
    currTime = high_resolution_clock::now();
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT)
        running = false;
    }
    velsys.update();
    pupdate.update();
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, &srcr, NULL);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //SDL2 segfaults if the size of the array is too big
    auto psize =  points.data.size();
    for(int i = 0; i < psize/1000;i++)
      SDL_RenderDrawPoints(renderer, points.data.data()+1000*i,1000);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
    auto elapsed = currTime - prevTime;
    state.dt = duration<double>(currTime-prevTime).count();
    prevTime = currTime;
    //cout << 1/state.dt << endl;
  }

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  for(int i = 0; i < esize; i++)
    delete entities[i];
  return 0;
}
