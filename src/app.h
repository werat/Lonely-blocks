// @werat

#ifndef _APP_H
#define _APP_H

#include <SDL.h>

class App 
{
private:
   bool isRunning = false;

protected:
   SDL_Window* window = nullptr;
   SDL_Renderer* renderer = nullptr;
   int width, height;

   int MaxFPS;

public:
   App();
   virtual ~App() {}

   void Run();
   
protected:

   virtual void OnInit();

   virtual void OnEvent(SDL_Event* event);

   // @delta - time elapsed from last update (in seconds)
   virtual void OnUpdate(float delta);

   // @delta - time elapsed from last draw (in seconds)
   virtual void OnRender(float delta);

   virtual void OnCleanup();

   void Exit();

   /// Some usefull functions

   void TakeScreenshot(std::string path = "");
};

#endif   