#pragma once

#include "ecs.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Vec2f {
  float x=0, y=0;
};

struct VelSys : public EntitySystem {
  Component<Vec2f>* pos;
  Component<Vec2f>* vel;

  VelSys(Component<Vec2f>* posc, Component<Vec2f>* velc,State * state);
  void entityUpdate(Entity * e);
};

struct AclSys : public EntitySystem {
  AclSys(State * state);
  void entityUpdate(Entity * e);
};

struct AnimationSys : public EntitySystem {
  AnimationSys(State * state);
  void entityUpdate(Entity * e);
};


struct PointRender : public System {
  PointRender(State * state);
  void update();
};
