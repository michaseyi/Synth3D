#pragma once
#include "Camera.hpp"
#include "../../external/ecs/ecs.hpp"


namespace ecs = ecs_hpp;

namespace Synth3D
{

   struct RenderEvent
   {
   };

   class RenderSystem : public ecs::system<RenderEvent>
   {
   };

}