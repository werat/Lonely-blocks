// @werat

#ifndef BLOCK_RENDERER
#define BLOCK_RENDERER

#include "component.h"

class BlockRenderer : public Component
{
public:
   virtual void Render(float delta, SDL_Renderer* renderer) override;

public:
   char r = 255, g = 255, b = 255, a = 255;
};

#endif