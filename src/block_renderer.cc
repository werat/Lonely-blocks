// @werat

#include <SDL.h>

#include "game_object.h"
#include "rigid_body.h"
#include "block_renderer.h"
#include "renderer.h"

void BlockRenderer::Render(float delta, SDL_Renderer* renderer)
{
   SDL_Rect bounds = Renderer::Bounds(gameObject().rigidBody());

   SDL_SetRenderDrawColor(renderer, r, g, b, a);
   SDL_RenderFillRect(renderer, &bounds);
   SDL_SetRenderDrawColor(renderer, 0, 225, 35, 255);
   SDL_RenderDrawRect(renderer, &bounds);
}