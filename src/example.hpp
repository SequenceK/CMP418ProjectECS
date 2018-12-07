#pragma once

#include "ecs.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Vec2f {
  float x=0, y=0;
};

struct VelSys : public EntitySystem {
  VelSys(State * state);
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
