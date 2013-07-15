// @werat

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "component.h"
#include "rigid_body.h"

#include <typeinfo>
#include <typeindex>
#include <map>
#include <string>

class GameObject
{
private:
   bool _initialized = false;
   std::map<std::type_index, Component*> _components;

public:
   std::string name;

public:
   GameObject(std::string name = "game_object");
   ~GameObject();

   // Adding component of type that is already attached will result in std::invalid_argument exception
   void AddComponent(Component* component);

   template<typename C> 
   C* GetComponent();

   // helper functions
   RigidBody* rigidBody();

   // Initialize all attached components
   void Init();
   void PrePhysicsUpdate(float delta);
   void Update(float delta);
};

template<typename C>
C* GameObject::GetComponent()
{
   auto it = _components.find(typeid(C));
   if (it == end(_components)) return nullptr;
   return dynamic_cast<C*>(it->second);
}

#endif