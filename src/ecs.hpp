#pragma once

#include <vector>

using namespace std;

#define ID unsigned int

struct State;

enum BType {
    ENTITY,
    COMPONENT,
    SYSTEM
};

struct Base {
    State * state;
    ID id;
    BType type;

    Base(State * state, ID id, BType type);
};

struct ComponentBase : Base {
    virtual void* get(ID index) = 0;
};

template<class T>
struct Component : ComponentBase {
    ID id;
    vector<T> data;

    void * get(ID index);
};

struct System : Base {
    vector<ID> entities;

    virtual void update() = 0;
    virtual void render() = 0;
};

struct Entity : Base {
    vector<ID> components;
    vector<ID> systems;

    Entity(State * state, ID id);
};

struct State {
    vector<Entity> entities;
    vector<ComponentBase*> components;
    vector<System*> systems;

    State();

    ID createEntity();
    template<class T>
    ID registerComponent(Component<T> * component);
    ID registerSystem(System * system);
};