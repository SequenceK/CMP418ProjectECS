#include "ecs.hpp"

#include <iostream>
#include <omp.h>
#include <map>
#include <queue>


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
}

System::System(State * state) : Base(state, BType::SYSTEM) {
  id = state->systems.size();
  state->systems.push_back(this);
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
  resolveDepGraph(root);
  dlist.sort([](const DepGraph* a,const DepGraph* b){return a->order<b->order;});

  for(DepGraph * d : dlist) {
    if(d->sys)
      runningQueue.push_back(d->sys);
  }

}

void State::update() {
  queueIndex = 0;

  for(auto & sys : runningQueue) {
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


DepGraph::DepGraph(){
 }

DepGraph::DepGraph( System* sys) {
  this->sys = sys;
}

DepGraph::DepGraph( ComponentBase* comp) {
  this->comp = comp;
}

list<DepGraph*> generateDepGraph(State * state) {
  map<ID, DepGraph*> graphsmap{};
  map<ID, DepGraph*> graphcmap{};
  list<DepGraph*> glist{};
  DepGraph * root = new DepGraph();
  glist.push_back(root);

  //Write dependencies pass
  for(System * sys : state->systems) {
    DepGraph * g = new DepGraph(sys);
    glist.push_back(g);
    graphsmap[sys->id] = g;
    for(ID id : sys->writeCompDeps) {
      ComponentBase * com = state->getComponent(id);
      DepGraph * cg;
      if(graphcmap.find(id) != graphcmap.end()){
        cg = graphcmap[id];
      }
      else {
        cg = new DepGraph(com);
        glist.push_back(cg);
        graphcmap[id] = cg;
      }
      g->childs.push_back(cg);
      cg->parents.push_back(g);
    }
  }

  //Read dependencies pass
  for(System * sys : state->systems) {
    DepGraph* g = graphsmap[sys->id];
    for(ID id : sys->readCompDeps) {
      ComponentBase * com = state->getComponent(id);
      DepGraph * cg;
      if(graphcmap.find(id) != graphcmap.end())
        cg = graphcmap[id];
      else{
        cg = new DepGraph(com);
        glist.push_back(cg);
        graphcmap[id] = cg;
      }

      cg->childs.push_back(g);
      g->parents.push_back(cg);
    }
  }

  //Pass over the graph map and add empty parent nodes to the root
  for(auto& pair : graphcmap) {
    DepGraph * node = pair.second;
    if(node->parents.size()==0) {
      node->parents.push_back(root);
      root->childs.push_back(node);
    }
  }
  for(auto& pair : graphsmap) {
    DepGraph * node = pair.second;
    if(node->parents.size()==0) {
      node->parents.push_back(root);
      root->childs.push_back(node);
    }
  }

  return glist;
}

 void resolveDepGraph( DepGraph * n) {
   for(DepGraph * c : n->childs) {
     if(n->sys)
       c->order++;
     else if(n->comp)
       c->order = (c->order<n->order)?n->order:c->order;

     resolveDepGraph(c);
   }
}

