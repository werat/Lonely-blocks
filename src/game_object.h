// @werat

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <string>
#include <stdexcept>

#include "common.h"
#include "component.h"
#include "rigid_body.h"

class Scene;

class GameObject
{
public:
   GameObject(Scene* scene, std::string name = "game_object");
   ~GameObject();

   // Adding component of type that is already attached will result in std::invalid_argument exception
   // This method initializes component if the game object has already been initialized
   template<typename C>
   C* AddComponent();

   template<typename C>
   C* GetComponent();

   // helper functions
   RigidBody& rigidBody() { return *_rigidBody; }
   Scene& scene() { return *_scene; }

   // Initialize all attached components
   void Init();
   void Destroy();

   void PrePhysicsUpdate(float delta);
   void Update(float delta);
   void Render(float delta, SDL_Renderer *renderer);

public:
   std::string name;

private:
   bool _initialized = false;
   bool _destroyed = false;

   std::unordered_map<std::type_index, Component*> _components;
   RigidBody* _rigidBody = nullptr;

   Scene* _scene;

   DISALLOW_COPY_AND_ASSIGN(GameObject);
};

template<>
RigidBody* GameObject::AddComponent<RigidBody>();

template<typename C>
C* GameObject::AddComponent()
{
   if (_components.find(typeid(C)) != end(_components)) {
      std::string type_name = typeid(C).name();
      throw std::invalid_argument("Component of type " + type_name + " is already attached.");
   }

   auto component = new C();

   _components[typeid(*component)] = component;
   component->_gameObject = this;

   if (_initialized)
   {
      component->Init();
   }

   return component;
}

template<typename C>
C* GameObject::GetComponent()
{
   auto it = _components.find(typeid(C));
   if (it == end(_components)) return nullptr;
   return static_cast<C*>(it->second);
}

#endif