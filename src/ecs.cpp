#include "ecs.hpp"

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

ComponentBase::ComponentBase(State * state) : Base(state, BType::COMPONENT) {
    id = state->components.size();
    state->components.push_back(this);
}

System::System(State * state) : Base(state, BType::SYSTEM) {
    id = state->systems.size();
    state->systems.push_back(this);
}



EntitySystem::EntitySystem(State * state) : System(state), entities() {
  
}

void EntitySystem::registerEntity(ID eid) {
  entities.push_back(eid);
}

State::State() : entities(), components(), systems() {

}


void State::update() {
    for(auto & sys : systems) {
        sys->update();
    }
}
