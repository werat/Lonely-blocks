// @werat

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <typeinfo>
#include <typeindex>
#include <map>
#include <string>
#include <stdexcept>

#include "common.h"
#include "component.h"
#include "rigid_body.h"
#include "physics_component.h"

class GameObject
{
public:
   GameObject(std::string name = "game_object");
   ~GameObject();

   // Adding component of type that is already attached will result in std::invalid_argument exception
   // This method initializes component if the game object has already been initialized
   template<typename C>
   C* AddComponent();

   template<typename C>
   C* GetComponent();

   // helper functions
   RigidBody* rigidBody();

   // Initialize all attached components
   void Init();
   void PrePhysicsUpdate(float delta);
   void Update(float delta);

public:
   std::string name;

private:
   bool _initialized = false;
   std::map<std::type_index, Component*> _components;

   RigidBody* _rigidBody = nullptr;

   DISALLOW_COPY_AND_ASSIGN(GameObject);
};

template<>
PhysicsComponent* GameObject::AddComponent<PhysicsComponent>();

template<typename C>
C* GameObject::AddComponent()
{
   if (_components.find(typeid(C)) != end(_components)) {
      std::string type_name = typeid(C).name();
      throw std::invalid_argument("Component of type " + type_name + " is already attached.");
   }

   auto component = new C();

   _components[typeid(*component)] = component;
   component->gameObject = this;

   if (_initialized)
   {
      component->Init();
   }

   return component;
}

// template<>
// inline PhysicsComponent* GameObject::AddComponent<PhysicsComponent>()
// {
//    if (_components.find(typeid(PhysicsComponent)) != end(_components)) {
//       throw std::invalid_argument("Physics Component is already attached.");
//    }

//    auto component = new PhysicsComponent();
//    _rigidBody = component->rigidBody;

//    _components[typeid(*component)] = component;
//    component->gameObject = this;

//    if (_initialized)
//    {
//       component->Init();
//    }

//    return component;
// }

template<typename C>
C* GameObject::GetComponent()
{
   auto it = _components.find(typeid(C));
   if (it == end(_components)) return nullptr;
   return static_cast<C*>(it->second);
}

#endif