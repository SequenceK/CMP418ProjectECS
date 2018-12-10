#pragma once

#include <vector>
#include <list>
#include <string>

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

struct Entity : public Base {
  ID * components;
  ID  * systems;

  Entity(State * state);
  ~Entity();

  void addComponent(ID cid, ID index);
  void addSystem(ID sid, ID index);
};
struct ComponentBase : public Base {

  ComponentBase(State * state);

  virtual void* getv(ID index) = 0;
};

template<class T>
struct Component : public ComponentBase {
  vector<T> data;

  Component(State * state) : ComponentBase(state) {};

  void * getv(ID index) {
    return (void*) &data[index];
  }

  T * get(Entity*e) {
    return &data[e->components[id]];
  }

  template<class... Args>
  T * create(Entity * e, Args... args) {
    ID cid = data.size();
    e->addComponent(id, cid);
    data.emplace_back(args...);
    return &data[cid];
  }
};

struct System : public Base {
  vector<ID> readCompDeps{};
  vector<ID> writeCompDeps{};
  System(State * state);

  virtual void update() = 0;

  void addReadCompDep(ID id);
  void addWriteCompDep(ID id);

};

struct EntitySystem : public System {
  vector<ID> entities;

  EntitySystem(State * state);
  void update();

  virtual void entityUpdate(Entity * e) = 0;
  void registerEntity(Entity * e);
};

typedef void (*FPtr)();

struct State {
  vector<Entity*> entities;
  vector<ComponentBase*> components;
  vector<System*> systems;

  double dt = 0;

  vector<System*> runningQueue{};
  vector<unsigned int> tasksPerSection{};
  unsigned int qIndex = 0;

  State();
  void init();
  void update();

  Entity* getEntity(ID id);
  ComponentBase* getComponent(ID id);
  System* getSystem(ID id);

};


struct DepGraph {
  System * sys;
  ComponentBase * comp;
  vector<DepGraph*> childs;
  DepGraph* parent;
  unsigned int resolveCount = 0;

  DepGraph();
  DepGraph(DepGraph*parent, System*sys);
  DepGraph(DepGraph*parent, ComponentBase*comp);
};


list<DepGraph*> generateDepGraph(State * state);
void resolveDepGraph(State * state, DepGraph * root);
