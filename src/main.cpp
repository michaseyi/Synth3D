
#include "Renderer/Shader.hpp"
#include "Core/Runtime.hpp"
#include "Core/Mesh.hpp"
#include "Utils/Utils.hpp"
#include "../external/ecs/ecs.hpp"
namespace ecs = ecs_hpp;

Synth3D::Runtime runtime("Synth3D", 800, 800);

int main()
{
   ecs::registry world;
   auto entity = world.create_entity();
   auto material = Synth3D::basic_material(glm::vec3(1.0f));
   auto box_geometry = Synth3D::Geometry();
   std::cout << "start" << std::endl;
   entity.assign_component<Synth3D::Mesh>(box_geometry, material);
   std::cout << "done" << std::endl;
   material.use();
   // Synth3D::Runtime::main_loop();
   return 0;
}