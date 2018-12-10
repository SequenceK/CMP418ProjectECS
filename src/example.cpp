#include "example.hpp"


VelSys::VelSys(Component<Vec2f>* posc, Component<Vec2f>* velc, State * state) : EntitySystem(state) {
  pos = posc;
  vel = velc;

  addReadCompDep(vel->id);
  addWriteCompDep(pos->id);
}
void VelSys::entityUpdate(Entity * e) {
  Vec2f * p = pos->get(e);
  Vec2f * v = vel->get(e);

  p->x += v->x*state->dt;
  p->y += v->y*state->dt;
}

SDLPointUpdate::SDLPointUpdate(Component<Vec2f>* posc,
                               Component<SDL_Point>* pointc,
                               State * state) : EntitySystem(state) {
  pos = posc;
  points = pointc;

  addReadCompDep(pos->id);
  addWriteCompDep(points->id);
}

void SDLPointUpdate::entityUpdate(Entity* e) {
  Vec2f * p = pos->get(e);
  SDL_Point* point = points->get(e);

  point->x = p->x;
  point->y = p->y;
};
