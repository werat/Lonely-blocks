// @werat

#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include "physical.h"

#include <vector>

class PhysicsEngine
{
private:
   std::vector<Physical*> physicals;

public:
   Vector2 gravity;
public:
   PhysicsEngine();
   ~PhysicsEngine();

   // TODO: very dangerous as physical can be soon destroyed and pointer would be invalid
   void AttachPhysical(Physical* physical);
   void DetachPhysical(Physical* physical);

   void Update(float delta);

private:
   void ResolveXThenY(Physical& physical, std::vector<Physical*>& c_physicals, const Vector2& resolution);
   void ResolveYThenX(Physical& physical, std::vector<Physical*>& c_physicals, const Vector2& resolution);
   void UpdatePhysicalState(Physical& physical, const Vector2& resolution);
};

#endif