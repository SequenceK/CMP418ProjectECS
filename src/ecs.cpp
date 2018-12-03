#include "ecs.hpp"

Base::Base(State* state, BType type) : state(state), type(type) {
}

Entity::Entity(State * state) : Base(state, BType::ENTITY) {
    id = state->entities.size();
    state->entities.push_back(this);
}

ComponentBase::ComponentBase(State * state) : Base(state, BType::COMPONENT) {
    id = state->components.size();
    state->components.push_back(this);
}

System::System(State * state) : Base(state, BType::SYSTEM), entities() {
    id = state->systems.size();
    state->systems.push_back(this);
}

void System::registerEntity(ID eid) {
    entities.push_back(eid);
}

State::State() : entities(), components(), systems() {

}


void State::update() {
    for(auto & sys : systems) {
        sys->update();
    }
}