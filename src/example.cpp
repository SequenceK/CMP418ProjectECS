#include "example.hpp"


VelSys::VelSys(Component<Vec2f>* posc, Component<Vec2f>* velc, State * state) : EntitySystem(state) {
  pos = posc;
  vel = velc;
}
void VelSys::entityUpdate(Entity * e) {
  Vec2f * p = pos->get(e);
  Vec2f * v = vel->get(e);

  p->x += v->x*state->dt;
  p->y += v->y*state->dt;
}


