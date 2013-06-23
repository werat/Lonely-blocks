// @werat

#ifndef RENDERER_H
#define RENDERER_H

#include <SDL.h>

class Renderer
{
public:

   /**
   *  Draw a SDL_Texture to the screen at dstRect with various other options
   *  @param tex The SDL_Texture to draw
   *  @param dstRect The destination position and width/height to draw the texture with
   *  @param srcRect The srcRect to apply to the image, if desired
   *  @param angle The rotation angle to apply to the texture, default is 0
   *  @param xPivot The x coordinate of the pivot, relative to (0, 0) being center of dstRect
   *  @param yPivot The y coordinate of the pivot, relative to (0, 0) being center of dstRect
   *  @param flip The flip to apply to the image, default is none
   */
   static void DrawTexture(SDL_Renderer *renderer, SDL_Texture *tex, SDL_Rect &dstRect, SDL_Rect *srcRect = NULL,
                           float angle = 0.0, int xPivot = 0, int yPivot = 0,
                           SDL_RendererFlip flip = SDL_FLIP_NONE);
};

#endif