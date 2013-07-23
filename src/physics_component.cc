// @werat

#include <SDL.h>

#include "collision.h"
#include "game_object.h"
#include "rigid_body.h"
#include "physics_component.h"
#include "block_renderer.h"

void PhysicsComponent::Init()
{
   gameObject().rigidBody().onCollision = [&](const CollisionInfo& info)
   {
      if (info.normal.y < 0 && info.moveDirection.y > 0)
      {
         if (info.other->gameObject().name == "jumpy_tile") return;

         onGround = true;
      }
   };
}
void PhysicsComponent::PrePhysicsUpdate(float delta)
{
   onGround = false;
}
void PhysicsComponent::Update(float delta)
{
   Vector2 input = Vector2::Zero;

   auto state = SDL_GetKeyboardState(NULL);
   if (state[SDL_SCANCODE_LEFT]) input.x = -1;
   if (state[SDL_SCANCODE_RIGHT]) input.x = 1;
   if (state[SDL_SCANCODE_UP]) input.y = -1;
   if (state[SDL_SCANCODE_DOWN]) input.y = 1;

   if (input.y < 0 && onGround) {
      gameObject().rigidBody().ApplyImpulse({0, -2300});
   }
   if (input.x != 0 ) {
      gameObject().rigidBody().ApplyForce({5000 * input.x, 0});
   }

   BlockRenderer* br = gameObject().GetComponent<BlockRenderer>();
   if (br != nullptr)
   {
      if (onGround) { br->r = 0; br->g = 128; br->b = 128; }
      else          { br->r = 255; br->g = 128; br->b = 128; }
   }
}