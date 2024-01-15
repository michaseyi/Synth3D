#include <iostream>
#include "../Math/glm.hpp"
#include "../Renderer/Buffer.hpp"
#include "Mesh.hpp"

namespace Synth3D
{

   class Component
   {
   public:
      virtual void serialize() = 0;
      virtual ~Component() = default;
   };

   class TransformComponent : public Component
   {
   public:
      glm::vec3 position{0.0f};
      glm::vec3 scale{1.0f};
      glm::quat orientation{};
      void serialize() override;
      ~TransformComponent() = default;
   };
   class MeshData : public Component
   {
   public:
      MeshData() = default;
      ~MeshData() = default;
      Mesh mesh;
      GeometryBuffer buffer;
      void update();
      void serialize() override;
   };

}