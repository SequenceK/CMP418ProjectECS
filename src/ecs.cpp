#include "ecs.hpp"

Base::Base(State* state, ID id, BType type) : state(state), id(id), type(type) {
    
}

Entity::Entity(State * state, ID id) : Base(state, id, BType::ENTITY) {

}

State::State() : entities(), components(), systems() {

}

ID State::createEntity() {
    ID id = entities.size();
    Entity e{this, id};
    entities.push_back(e);
    return id;
}