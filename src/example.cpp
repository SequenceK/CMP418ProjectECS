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

AclSys::AclSys(Component<Vec2f>*velc, Component<Vec2f>*aclc, State*state) : EntitySystem(state) {
  vel = velc;
  acl = aclc;

  addReadCompDep(acl->id);
  addWriteCompDep(vel->id);
}

void AclSys::entityUpdate(Entity * e) {
  Vec2f * v = vel->get(e);
  Vec2f * a = acl->get(e);

  v->x += a->x*state->dt;
  v->y += a->y*state->dt;
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

BoundsChecker::BoundsChecker(Component<Vec2f>*posc, Component<Vec2f>*velc, State*state) : EntitySystem(state) {
  pos = posc;
  vel = velc;

  addWriteCompDep(pos->id);
  addWriteCompDep(vel->id);
}

void BoundsChecker::entityUpdate(Entity * e) {
  Vec2f * p = pos->get(e);
  Vec2f * v = vel->get(e);
  bool invertx = false;
  bool inverty = false;
  if(p->x < 0) {
    p->x = 0;
    invertx = true;
  } else if(p->x > state->width) {
    p->x = state->width;
    invertx = true;
  }

  if(p->y < 0) {
    p->y = 0;
    inverty = true;
  } else if(p->y > state->height) {
    p->y = state->height;
    inverty = true;
  }

  if(invertx)
    v->x *= -1;
  if(inverty)
    v->y *= -1;
}
