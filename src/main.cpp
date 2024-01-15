
#include "Core/Runtime.hpp"
#include "Core/Mesh.hpp"
#include "Core/Scene.hpp"
#include "Core/Geometry.hpp"

using namespace Synth3D;

int main()
{
   Runtime &runtime = Runtime::get_instance();

   auto sphere = new Node("sphere");
   auto transform_component = new TransformComponent();
   auto sphere_mesh_data = new MeshData();
   add_sphere(*sphere_mesh_data, 1.0f, 16, 32);
   sphere->add_component(transform_component);
   sphere->add_component(sphere_mesh_data);
   runtime.scene.add(sphere);

   auto knot = new Node("knot");
   transform_component = new TransformComponent();
   transform_component->position = glm::vec3(-3.0f, 0.0f, 0.0f);
   auto knot_mesh_data = new MeshData();
   add_knot(*knot_mesh_data, 0.3f, 0.2f, 64, 16);
   knot->add_component(transform_component);
   knot->add_component(knot_mesh_data);
   runtime.scene.add(knot);

   auto cube = new Node("cube");
   transform_component = new TransformComponent();
   transform_component->position = glm::vec3(3.0f, 0.0f, 0.0f);
   transform_component->orientation = glm::angleAxis(glm::radians(30.0f), glm::normalize(glm::vec3(1.0f, 3.0f, 2.0f)));
   auto cube_mesh_data = new MeshData();
   add_cube(*cube_mesh_data);
   cube->add_component(transform_component);
   cube->add_component(cube_mesh_data);
   runtime.scene.add(cube);

   auto cylinder = new Node("cylinder");
   auto cylinder_mesh_data = new MeshData();
   add_cylinder(*cylinder_mesh_data, 1.0f, 2.0f, 30);
   cylinder->add_component(cylinder_mesh_data);
   transform_component = new TransformComponent();
   transform_component->position = glm::vec3(0.0f, 1.0f, 30.0f);
   cylinder->add_component(transform_component);
   runtime.scene.add(cylinder);
 
   while (runtime.is_open())
   {
      runtime.update();
   }

   return 0;
}
