// @werat

#include "game_object.h"
#include "physics_component.h"

#include <map>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <stdexcept>
#include <iterator>

GameObject::GameObject(std::string name)
{
   this->name = name;
}
GameObject::~GameObject()
{
   for (auto pair : _components)
   {
      delete pair.second;
   }
}

void GameObject::AddComponent(Component* component)
{
   if (_components.find(typeid(*component)) != end(_components)) {
      std::string type_name = typeid(*component).name();
      throw std::invalid_argument("Component of type " + type_name + " already attached.");
   }

   _components[typeid(*component)] = component;
   component->gameObject = this;

   if (_initialized)
   {
      component->Init();
   }
}

RigidBody* GameObject::rigidBody()
{
   auto physics = GetComponent<PhysicsComponent>();
   if (physics == nullptr) return nullptr;
   return physics->rigidBody;
}

void GameObject::Init()
{
   if (_initialized) return;

   for (auto pair : _components)
   {
      pair.second->Init();
   }
   _initialized = true;
}
void GameObject::PrePhysicsUpdate(float delta)
{
   for (auto pair : _components)
   {
      pair.second->PrePhysicsUpdate(delta);
   }
}
void GameObject::Update(float delta)
{
   for (auto pair : _components)
   {
      pair.second->Update(delta);
   }
}
