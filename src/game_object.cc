// @werat

#include <typeinfo>
#include <typeindex>
#include <string>
#include <stdexcept>
#include <iterator>

#include "game_object.h"
#include "scene.h"

GameObject::GameObject(Scene* scene, std::string name)
{
   if (scene == nullptr) {
      throw std::invalid_argument("scene can't be nullptr.");
   }

   this->_scene = scene;
   this->_name = name;

   _scene->Add(this);
}
GameObject::GameObject(GameObject* parent, std::string name)
{
   if (parent == nullptr) {
      throw std::invalid_argument("parent can't be nullptr.");
   }

   this->_parent = parent;
   _parent->_children.push_back(this);
   this->_scene = parent->_scene;
   this->_name = name;
}
GameObject::~GameObject()
{
   if (!_destroyed)
   {
      Destroy();
   }
   for (auto pair : _components)
   {
      delete pair.second;
   }
}

template<>
RigidBody* GameObject::AddComponent<RigidBody>()
{
   if (_rigidBody != nullptr) {
      throw std::invalid_argument("Rigidbody is already attached.");
   }

   _rigidBody = _scene->physics().CreateBody();
   _rigidBody->_gameObject = this;

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
   for (auto child : _children)
   {
      child->Init();
   }
   _initialized = true;
}
void GameObject::Destroy()
{
   if (_destroyed) {
      throw std::logic_error("The game object has already been destroyed.");
   }

   for (auto child : _children)
   {
      child->Destroy();
   }
   for (auto pair : _components)
   {
      pair.second->Destroy();
   }
   if (_rigidBody != nullptr)
   {
      _scene->physics().DestroyBody(_rigidBody);
   }

   _destroyed = true;
}
void GameObject::PrePhysicsUpdate(float delta)
{
   for (auto pair : _components)
   {
      pair.second->PrePhysicsUpdate(delta);
   }
   for (auto child : _children)
   {
      child->PrePhysicsUpdate(delta);
   }
}
void GameObject::Update(float delta)
{
   for (auto pair : _components)
   {
      pair.second->Update(delta);
   }
   for (auto child : _children)
   {
      child->Update(delta);
   }
}
void GameObject::Render(float delta, SDL_Renderer *renderer)
{
   for (auto pair : _components)
   {
      pair.second->Render(delta, renderer);
   }
   for (auto child : _children)
   {
      child->Render(delta, renderer);
   }
}