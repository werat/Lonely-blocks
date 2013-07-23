// @werat

#include <stdexcept>

#include "game_object.h"
#include "scene.h"
#include "component.h"
#include "physics_engine.h"

inline PhysicsEngine& Component::physics()
{
   return _gameObject->scene().physics();
}