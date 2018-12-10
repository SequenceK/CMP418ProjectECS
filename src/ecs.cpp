#include "ecs.hpp"

#include <iostream>
#include <omp.h>
#include <map>
#include <queue>

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

void System::addReadCompDep(ID id) {
  readCompDeps.push_back(id);
}

void System::addWriteCompDep(ID id) {
  writeCompDeps.push_back(id);
}

EntitySystem::EntitySystem(State * state) : System(state), entities() {
}

void EntitySystem::update() {
#pragma omp parallel for
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

void State::init() {
  list<DepGraph*> dlist = generateDepGraph(this);
  DepGraph * root = dlist.front();
  resolveDepGraph(this, root);
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


DepGraph::DepGraph() : childs() {
  parent = nullptr;
  sys = nullptr;
  comp = nullptr;
}
DepGraph::DepGraph(DepGraph*parent, System* sys) : childs() {
  parent = parent;
  sys = sys;
  comp = nullptr;
}

DepGraph::DepGraph(DepGraph*parent, ComponentBase* comp) : childs() {
  parent = parent;
  sys = nullptr;
  comp = comp;
}


list<DepGraph*> generateDepGraph(State * state) {
  map<ID, DepGraph*> graphmap{};
  list<DepGraph*> glist{};
  DepGraph * root = new DepGraph();
  glist.push_back(root);

  //Write dependencies pass
  for(System * sys : state->systems) {
    DepGraph * g = new DepGraph(nullptr, sys);
    glist.push_back(g);
    graphmap[sys->id] = g;
    if(sys->writeCompDeps.size()>1) {

      for(ID id : sys->writeCompDeps) {
        ComponentBase * com = state->getComponent(id);
        DepGraph * cg;
        if(graphmap.find(id) != graphmap.end()){
          cg = graphmap[id];
          if(cg->parent!=nullptr) {
            g->parent = cg->parent;
            cg->parent = g;
          }
          else
            cg->parent = g;
        }
        else {
          cg = new DepGraph(g, com);
          glist.push_back(cg);
          g->childs.push_back(cg);
        }
      }
    }
  }

  //Read dependencies pass
  for(System * sys : state->systems) {
    DepGraph* g = graphmap[sys->id];
    if(sys->readCompDeps.size()>1) {

      for(ID id : sys->readCompDeps) {
        ComponentBase * com = state->getComponent(id);
        DepGraph * cg;
        if(graphmap.find(id) != graphmap.end())
          cg = graphmap[id];
        else{
          cg = new DepGraph(nullptr, com);
          glist.push_back(cg);
        }

        cg->childs.push_back(g);
        g->resolveCount++;
      }
    }
  }

  //Pass over the graph map and add empty parent nodes to the root
  for(auto& pair : graphmap) {
    DepGraph * node = pair.second;
    if(node->parent==nullptr)
      node->parent = root;
  }

  return glist;
}

void resolveDepGraph(State * state, DepGraph * root) {
  queue<DepGraph*>unresolved{};
  unresolved.push(root);

  while(unresolved.size()!=0) {
    int sectionCount = 0;
    DepGraph * cnode = unresolved.front();
    unresolved.pop();

    for(DepGraph * n : cnode->childs) {
      n->resolveCount--;
      if(n->sys) {
        if(n->resolveCount <= 0) {
          sectionCount++;
          state->runningQueue.push_back(n->sys);
          for(DepGraph* n2 : n->childs) {
            unresolved.push(n2);
          }
        }
      } else {
        for(DepGraph* n2 : n->childs) {
          n2->resolveCount--;
          if(n2->sys && n2->resolveCount <= 0) {
            sectionCount++;
            state->runningQueue.push_back(n2->sys);
            for(DepGraph* n2 : n->childs) {
              unresolved.push(n2);
            }
          }
        }
      }
    }
  }
}

