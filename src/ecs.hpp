#pragma once

#include <vector>
#include <list>

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

    Base(State * state, BType type);
};


struct ComponentBase : public Base {

    ComponentBase(State * state);

    virtual void* get(ID index) = 0;
};

template<class T>
struct Component : public ComponentBase {
    vector<T> data;

    Component(State * state) : ComponentBase(state) {};

    void * get(ID index) {
        return (void*) &data[index];
    }

    T * createComponent(ID id);
};

struct System : public Base {
    vector<ID> entities;

    System(State * state);

    virtual void update() = 0;

    void registerEntity(ID eid);
};

struct Entity : public Base {
    ID * components;
    ID  * systems;

    Entity(State * state);
    ~Entity();
};

struct State {
    vector<Entity*> entities;
    vector<ComponentBase*> components;
    vector<System*> systems;

    bool systemsSorted = false;

    State();
    void update();

};

struct DepGraph {
    Base * base;
    list<DepGraph*> arrowTo;
    list<DepGraph*> arrowFrom;
};

struct SystemsJob {
    list<System*> systems;
};

list<SystemsJob> generateJobs(State * state);