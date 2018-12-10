#include "ecs.hpp"

#include <iostream>

#define tstr(x) #x

Base::Base(State* state, BType type) : state(state), type(type) {
}

Entity::Entity(State * state) : Base(state, BType::ENTITY) {
    id = state->entities.size();
    state->entities.push_back(this);

    auto csize = state->components.size();
    components = new ID[csize];
    for(int i = 0; i < csize; i++) {
      components[i] = -1;
    }

    auto ssize = state->systems.size();
    systems = new ID[state->systems.size()];
    for(int i = 0; i < ssize; i++) {
      systems[i] = -1;
    }
}

Entity::~Entity() {
  delete [] components;
  delete [] systems;
}

void Entity::addComponent(ID cid, ID dataindex){
  components[cid] = dataindex;
}

void Entity::addSystem(ID sid, ID index) {
  systems[sid] = index;
}

ComponentBase::ComponentBase(State * state) : Base(state, BType::COMPONENT) {
    id = state->components.size();
    state->components.push_back(this);

    cout << "Component " << id << " added.\n";
}

System::System(State * state) : Base(state, BType::SYSTEM) {
    id = state->systems.size();
    state->systems.push_back(this);

    cout << "System " << id << " added.\n";
}



EntitySystem::EntitySystem(State * state) : System(state), entities() {
}

void EntitySystem::update() {
  #pragma omp for
  for(int i = 0; i < entities.size(); i++) {
    auto & e = entities[i];
    entityUpdate(state->entities[e]);
  }
}

void EntitySystem::registerEntity(Entity* e) {
  e->addSystem(id, entities.size());
  entities.push_back(e->id);
}

State::State() : entities(), components(), systems() {

}


void State::update() {
    for(auto & sys : systems) {
        sys->update();
    }
}


Entity* State::getEntity(ID id){
  return entities[id];
}

ComponentBase* State::getComponent(ID id){
  return components[id];
}

System* State::getSystem(ID id) {
  return systems[id];
}
