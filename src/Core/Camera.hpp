#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Synth3D::Core
{
   class Camera
   {
   public:
      Camera(glm::vec3 position, glm::vec3 target, glm::vec3 orientation);
      virtual glm::mat4 LookAt();

   private:
      glm::vec3 position;
      glm::vec3 target;
      glm::quat orientation;
   };
}