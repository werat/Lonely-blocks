// @werat

#include "renderer.h"

void Renderer::DrawTexture(SDL_Renderer *renderer, SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *srcRect,
                           float angle, int xPivot, int yPivot,
                           SDL_RendererFlip flip)
{
   //Convert pivot pos from relative to object's center to screen space
    xPivot += dstRect.w / 2;
    yPivot += dstRect.h / 2;
    //SDL expects an SDL_Point as the pivot location
    SDL_Point pivot = { xPivot, yPivot };
    //Draw the texture
    SDL_RenderCopyEx(renderer, tex, srcRect, &dstRect, angle, &pivot, flip);
}