#include <SDL2/SDL.h>
#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <chrono>

#include "ecs.hpp"
#include "example.hpp"

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv) {

  if(argc > 1){
    int numthreads = stoi(argv[1]);
    omp_set_num_threads(numthreads);
  }
  int running = -1;
  if(argc > 2)
    running = stoi(argv[2]);

 

  int width = 500;
  int height = 500;

  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window * window;
  window = SDL_CreateWindow("Test",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            width,
                            height,
                            SDL_WINDOW_SHOWN
                            );
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_GL_SetSwapInterval(0);


  State state{};

  state.width = width;
  state.height = height;

  Component<Vec2f> pos(&state);
  Component<Vec2f> vel(&state);
  Component<Vec2f> acl(&state);
  Component<SDL_Point> points(&state);

  
  VelSys velsys(&pos, &vel, &state);
  BoundsChecker bcker(&pos, &vel, &state);
  AclSys aclsys(&vel, &acl, &state);
  SDLPointUpdate pupdate(&pos, &points, &state);
  

  unsigned int esize = 100000;
  if(argc > 3){
    esize = stoi(argv[3]);
  }
  Entity * entities[esize];

  for(int i = 0; i < esize; i++) {
    Entity * e  = new Entity(&state);
    entities[i] = e;
    Vec2f * p1 = pos.create(e);
    Vec2f * v1 = vel.create(e);
    Vec2f * a1 = acl.create(e);
    SDL_Point * point = points.create(e);
    a1->x = ((float)rand() / RAND_MAX - 0.5f)*10;
    a1->y = ((float)rand() / RAND_MAX - 0.5f)*10;
    v1->x = ((float)rand() / RAND_MAX - 0.5f)*10;
    v1->y = ((float)rand() / RAND_MAX - 0.5f)*10;
    p1->x = width/2.0f;
    p1->y = height/2.0f;
    aclsys.registerEntity(e);
    velsys.registerEntity(e);
    pupdate.registerEntity(e);
    bcker.registerEntity(e);
  }

  //bool running = true;
  SDL_Event e;

  state.init();

  auto currTime = high_resolution_clock::now();
  auto prevTime = currTime;
  while(running!=0) {

    running--;
    if(running<0)
      running=-1;

    currTime = high_resolution_clock::now();

    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT)
        running = 0;
    }

    state.update();
    auto elapsed = currTime - prevTime;
    state.dt = duration<double>(currTime-prevTime).count();
    prevTime = currTime;

    SDL_RenderClear(renderer);



    //SDL2 segfaults if the size of the array is too big
    auto psize =  points.data.size();
    auto ssize = 1000;
    for(int i = 0; i < psize/ssize;i++) {
      SDL_SetRenderDrawColor(renderer,
                             255*(i%10)/4,
                             255*((i+1)%10)/4,
                             255*((i+4)%10)/4, 255);
      SDL_RenderDrawPoints(renderer, points.data.data()+ssize*i,ssize);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    
    SDL_RenderPresent(renderer);


    

    //output the fps
    cout << 1.f/state.dt << endl;
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  SDL_Quit();

  for(int i = 0; i < esize; i++)
    delete entities[i];
  return 0;
}
