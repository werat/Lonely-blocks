// @werat

#include <map>
#include <typeinfo>
#include <typeindex>
#include <string>
#include <stdexcept>
#include <iterator>

#include "game_object.h"
#include "physics_component.h"

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

template<>
PhysicsComponent* GameObject::AddComponent<PhysicsComponent>()
{
   if (_components.find(typeid(PhysicsComponent)) != end(_components)) {
      throw std::invalid_argument("Physics Component is already attached.");
   }

   auto component = new PhysicsComponent();
   _rigidBody = component->rigidBody;

   _components[typeid(*component)] = component;
   component->gameObject = this;

   if (_initialized)
   {
      component->Init();
   }

   return component;
}

RigidBody* GameObject::rigidBody()
{
   if (_rigidBody == nullptr) {
      throw std::logic_error("Physics Component is not attached to the game object.");
   }
   return _rigidBody;
}

void GameObject::Init()
{
   if (_initialized) {
      throw std::logic_error("The game object has already been initialized.");
   }

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
