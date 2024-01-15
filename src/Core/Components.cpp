#include "Components.hpp"
namespace Synth3D
{

   void TransformComponent::serialize() {}

   void MeshData::serialize() {}

   void MeshData::update()
   {
      std::vector<float> position;
      std::vector<float> normals;
      std::vector<float> smooth_normals;

      position.reserve(mesh.n_faces() * 3 * 3);
      normals.reserve(mesh.n_faces() * 3 * 3);
      smooth_normals.reserve(mesh.n_faces() * 3 * 3);

      for (auto f_it = mesh.faces_begin(); f_it != mesh.faces_end(); f_it++)
      {
         auto normal = mesh.calc_face_normal(*f_it);
         for (auto v_it = (*f_it).vertices().begin(); v_it != (*f_it).vertices().end(); v_it++)
         {

            auto point = mesh.point(*v_it);
            position.emplace_back(point[0]);
            position.emplace_back(point[1]);
            position.emplace_back(point[2]);

            normals.emplace_back(normal[0]);
            normals.emplace_back(normal[1]);
            normals.emplace_back(normal[2]);

            auto smooth_normal = Mesh::Normal(0);
            int count = 0;

            for (auto sf_it : v_it->faces())
            {
               smooth_normal += mesh.calc_face_normal(sf_it);
               count++;
            }
            smooth_normal /= count;
            smooth_normal = smooth_normal.normalize();

            smooth_normals.emplace_back(smooth_normal[0]);
            smooth_normals.emplace_back(smooth_normal[1]);
            smooth_normals.emplace_back(smooth_normal[2]);
         }
      }
      buffer.create_vbos(3);
      buffer.vertex_count = mesh.n_faces() * 3;
      buffer.buffer_data<float>(position.data(), position.size(), 0);
      buffer.set_attrib_pointer<float>(0, 3, 0, 0, 0);
      buffer.buffer_data(normals.data(), normals.size(), 1);
      buffer.set_attrib_pointer<float>(1, 3, 0, 0, 1);
      buffer.buffer_data(smooth_normals.data(), smooth_normals.size(), 2);
      buffer.set_attrib_pointer<float>(2, 3, 0, 0, 2);
   }

}