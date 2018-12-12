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

  Component<Vec2f> * vel;
  Component<Vec2f> * acl;

  AclSys(Component<Vec2f>*velc, Component<Vec2f>*aclc,State * state);
  void entityUpdate(Entity * e);
};

struct SDLPointUpdate : public EntitySystem {
  Component<Vec2f>* pos;
  Component<SDL_Point>* points;

  SDLPointUpdate(Component<Vec2f>* posc,
                 Component<SDL_Point>* pointc,
                 State * state);
  void entityUpdate(Entity* e);
};

struct BoundsChecker : public EntitySystem {
  Component<Vec2f>* pos;
  Component<Vec2f>* vel;

  BoundsChecker(Component<Vec2f>*posc, Component<Vec2f>*velc, State*state);
  void entityUpdate(Entity* e);
};
