// @werat

#include <stdexcept>

#include "component.h"
#include "physics_engine.h"

bool Component::_global_initialized = false;
PhysicsEngine* Component::_physicsEngine = nullptr;

void Component::Initialize(PhysicsEngine* engine)
{
   if (_global_initialized) {
      throw std::logic_error("Global initialization of Component has already been done.");
   }

   _physicsEngine = engine;

   _global_initialized = true;
}

PhysicsEngine& Component::Physics()
{
   if (!_global_initialized) {
      throw std::logic_error("Component is not globally initialized.");
   }
   return *_physicsEngine;
}

