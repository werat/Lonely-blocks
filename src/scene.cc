// @werat

#include <SDL.h>
#include <algorithm>
#include <iterator>
#include <stdexcept>

#include "scene.h"

void Scene::Init()
{
   if (_initialized) {
      throw std::logic_error("The scene has already been initialized.");
   }

   for (auto gameObject : _gameObjects)
   {
      gameObject->Init();
   }
   _initialized = true;
}
void Scene::Destroy()
{
   if (_destroyed) {
      throw std::logic_error("The scene has already been destroyed.");
   }

   for (auto gameObject : _gameObjects)
   {
      gameObject->Destroy();
   }
   _destroyed = true;
}
void Scene::Update(float delta)
{
   for (auto gameObject : _gameObjects)
   {
      gameObject->PrePhysicsUpdate(delta);
   }

   _physics.Update(delta);
   
   for (auto gameObject : _gameObjects)
   {
      gameObject->Update(delta);
   }
}
void Scene::Render(float delta, SDL_Renderer *renderer)
{
   for (auto gameObject : _gameObjects)
   {
      gameObject->Render(delta, renderer);
   }  
}

void Scene::Add(GameObject* object)
{
   if (object == nullptr)
      throw std::invalid_argument("The object argument can't be null.");

   auto obj_it = std::find(begin(_gameObjects), end(_gameObjects), object);
   if (obj_it != end(_gameObjects))
      throw std::logic_error("The object is already attached.");

   _gameObjects.push_back(object);
}
void Scene::Remove(GameObject* object)
{
   if (object == nullptr)
      throw std::invalid_argument("The object argument can't be null.");

   auto obj_it = std::find(begin(_gameObjects), end(_gameObjects), object);
   if (obj_it == end(_gameObjects))
      throw std::logic_error("The object is not attached.");

   _gameObjects.erase(obj_it);
}