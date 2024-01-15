#include "Geometry.hpp"

namespace Synth3D
{
   void add_cube(MeshData &mesh_data)
   {
      Mesh::VertexHandle vhandles[8];

      // Define the 8 vertices of the cube
      vhandles[0] = mesh_data.mesh.add_vertex(Mesh::Point(-1, -1, 1));
      vhandles[1] = mesh_data.mesh.add_vertex(Mesh::Point(1, -1, 1));
      vhandles[2] = mesh_data.mesh.add_vertex(Mesh::Point(1, 1, 1));
      vhandles[3] = mesh_data.mesh.add_vertex(Mesh::Point(-1, 1, 1));
      vhandles[4] = mesh_data.mesh.add_vertex(Mesh::Point(-1, -1, -1));
      vhandles[5] = mesh_data.mesh.add_vertex(Mesh::Point(1, -1, -1));
      vhandles[6] = mesh_data.mesh.add_vertex(Mesh::Point(1, 1, -1));
      vhandles[7] = mesh_data.mesh.add_vertex(Mesh::Point(-1, 1, -1));

      std::vector<Mesh::VertexHandle> face_vhandles;

      // Define the 12 triangles of the cube
      std::vector<int> face_indices = {0, 1, 2, 0, 2, 3, 1, 5, 6, 1, 6, 2, 5, 4, 7, 5, 7, 6, 4, 0, 3, 4, 3, 7, 3, 2, 6, 3, 6, 7, 4, 5, 1, 4, 1, 0};
      for (int i = 0; i < 36; i += 3)
      {
         face_vhandles.clear();
         for (int j = 0; j < 3; ++j)
         {
            face_vhandles.push_back(vhandles[face_indices[i + j]]);
         }

         mesh_data.mesh.add_face(face_vhandles);
      }

      mesh_data.update();
   }

   void add_sphere(MeshData &mesh_data, float radius, int latitude_segments, int longitude_segments)
   {
      const float PI = 3.14159265358979323846f;

      // Create vertices
      std::vector<std::vector<Mesh::VertexHandle>> vhandles;
      for (int i = 0; i < longitude_segments; ++i)
      {
         vhandles.push_back({});
         float theta = i * 2 * PI / longitude_segments;
         for (int j = 1; j < latitude_segments; ++j)
         {
            float phi = (j * PI / latitude_segments) - (PI / 2.0f);
            float x = radius * cos(phi) * cos(-theta);
            float z = radius * cos(phi) * sin(-theta);
            float y = radius * sin(phi);
            vhandles[i].push_back(mesh_data.mesh.add_vertex(Mesh::Point(x, y, z)));
         }
      }



      
      auto top_center = mesh_data.mesh.add_vertex(Mesh::Point(radius * cos(glm::radians(90.0f)), radius * sin(glm::radians(90.0f)), 0.0f));
      auto bottom_center = mesh_data.mesh.add_vertex(Mesh::Point(radius * cos(glm::radians(90.0f)), radius * sin(glm::radians(-90.0f)), 0.0f));

      for (int i = 0; i < longitude_segments; ++i)
      {
         auto max_index = vhandles[i].size() - 1;
         std::vector<Mesh::VertexHandle> face_vhandles;
         face_vhandles.push_back(vhandles[i][max_index]);
         face_vhandles.push_back(vhandles[(i + 1) % longitude_segments][max_index]);
         face_vhandles.push_back(top_center);
         mesh_data.mesh.add_face(face_vhandles);

         face_vhandles.clear();
         face_vhandles.push_back(vhandles[i][0]);
         face_vhandles.push_back(bottom_center);
         face_vhandles.push_back(vhandles[(i + 1) % longitude_segments][0]);
         mesh_data.mesh.add_face(face_vhandles);
      }

      // Create faces
      for (int i = 0; i < vhandles.size(); ++i)
      {
         for (int j = 0; j < vhandles[0].size() - 1; ++j)
         {
            int next_i = (i + 1) % longitude_segments;
            int next_j = (j + 1);

            std::vector<Mesh::VertexHandle> face_vhandles;
            face_vhandles.push_back(vhandles[i][j]);
            face_vhandles.push_back(vhandles[next_i][j]);
            face_vhandles.push_back(vhandles[next_i][next_j]);
            mesh_data.mesh.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(vhandles[i][j]);
            face_vhandles.push_back(vhandles[next_i][next_j]);
            face_vhandles.push_back(vhandles[i][next_j]);
            mesh_data.mesh.add_face(face_vhandles);
         }
      }
      mesh_data.update();
   }

   void add_knot(MeshData &mesh_data, float radius, float thickness, int num_segments, int num_slices)
   {
      const float PI = 3.14159265358979323846f;

      // Create vertices
      std::vector<std::vector<Mesh::VertexHandle>> vhandles(num_segments + 1);
      for (int i = 0; i <= num_segments; ++i)
      {
         float t = i * (2 * PI) / num_segments;
         float x = radius * (sin(t) + 2 * sin(2 * t));
         float y = radius * (cos(t) - 2 * cos(2 * t));
         float z = -radius * sin(3 * t);
         glm::vec3 tangent(radius * (cos(t) + 4 * cos(2 * t)), radius * (-sin(t) + 4 * sin(2 * t)), -3 * radius * cos(3 * t));

         glm::vec3 rotation_axis = glm::cross(glm::vec3(0, 0, 1.0f), glm::normalize(tangent));
         float rotation_angle = glm::acos(glm::dot(normalize(tangent), glm::vec3(0, 0, 1.0f)));

         glm::mat4 rotation_matrix{1.0f};
         rotation_matrix = glm::rotate(rotation_matrix, rotation_angle, rotation_axis);
         // Create a circle of vertices around each point on the curve
         for (int j = 0; j < num_slices; ++j)
         {
            float slice_angle = j * (2 * PI) / num_slices;
            float slice_x = thickness * cos(slice_angle);
            float slice_y = thickness * sin(slice_angle);
            glm::vec4 position{thickness * cos(slice_angle), thickness * sin(slice_angle), 0, 1};
            position = rotation_matrix * position;
            vhandles[i].push_back(mesh_data.mesh.add_vertex(Mesh::Point(position.x + x, position.y + y, position.z + z)));
         }
      }

      // Create faces
      for (int i = 0; i < num_segments; ++i)
      {
         for (int j = 0; j < num_slices; ++j)
         {
            // First triangle
            mesh_data.mesh.add_face(vhandles[i][j],
                                    vhandles[(i + 1) % (num_segments + 1)][(j + 1) % num_slices],
                                    vhandles[(i + 1) % (num_segments + 1)][j]);

            // Second triangle
            mesh_data.mesh.add_face(vhandles[i][j],
                                    vhandles[i][(j + 1) % num_slices],
                                    vhandles[(i + 1) % (num_segments + 1)][(j + 1) % num_slices]

            );
         }
      }

      mesh_data.update();
   }
   void add_cylinder(MeshData &mesh_data, float radius, float height, int num_segments)
   {
      const float PI = 3.14159265358979323846f;

      // Create vertices for the top and bottom circles
      std::vector<Mesh::VertexHandle> bottom_vhandles, top_vhandles;
      for (int i = 0; i < num_segments; ++i)
      {
         float t = i * (2 * PI) / num_segments;
         float x = radius * cos(t);
         float y = radius * sin(t);

         bottom_vhandles.push_back(mesh_data.mesh.add_vertex(Mesh::Point(x, y, -height / 2.0f)));
         top_vhandles.push_back(mesh_data.mesh.add_vertex(Mesh::Point(x, y, height / 2.0f)));
      }

      Mesh::VertexHandle bottom_center = mesh_data.mesh.add_vertex(Mesh::Point(0, 0, -height / 2.0f));
      Mesh::VertexHandle top_center = mesh_data.mesh.add_vertex(Mesh::Point(0, 0, height / 2.0f));
      // Create faces for the top and bottom circles
      for (int i = 0; i < num_segments; ++i)
      {
         // Bottom face
         mesh_data.mesh.add_face(bottom_vhandles[i],
                                 bottom_center,
                                 bottom_vhandles[(i + 1) % num_segments]);
         // Top face
         mesh_data.mesh.add_face(top_vhandles[i],
                                 top_vhandles[(i + 1) % num_segments],
                                 top_center);
      }

      // Create faces for the sides of the cylinder
      for (int i = 0; i < num_segments; ++i)
      {
         // First triangle
         mesh_data.mesh.add_face(bottom_vhandles[i],
                                 bottom_vhandles[(i + 1) % num_segments],
                                 top_vhandles[i]);

         // Second triangle
         mesh_data.mesh.add_face(bottom_vhandles[(i + 1) % num_segments],
                                 top_vhandles[(i + 1) % num_segments],
                                 top_vhandles[i]);
      }

      mesh_data.update();
   }

   GeometryBuffer *create_helper_grid(float size, int divisions)
   {
      float step = size / divisions;
      float half = size / 2;

      GeometryBuffer *buffer = new GeometryBuffer();
      std::vector<glm::vec3> vertices;
      vertices.reserve((divisions + 1) * 4);

      auto next = -half + ((divisions / 2) * step);
      vertices.emplace_back(glm::vec3(-half, 0.0f, next));
      vertices.emplace_back(glm::vec3(half, 0.0f, next));

      vertices.emplace_back(glm::vec3(next, 0.0f, -half));
      vertices.emplace_back(glm::vec3(next, 0.0f, half));

      for (int i = 0; i <= divisions; i++)
      {
         // x
         auto next = -half + i * step;
         if (i == divisions / 2)
         {
            continue;
         }
         vertices.emplace_back(glm::vec3(-half, 0.0f, next));
         vertices.emplace_back(glm::vec3(half, 0.0f, next));
         // z

         vertices.emplace_back(glm::vec3(next, 0.0f, -half));
         vertices.emplace_back(glm::vec3(next, 0.0f, half));
      }
      buffer->create_vbos(1);
      buffer->buffer_data(reinterpret_cast<float *>(vertices.data()), vertices.size() * 3, 0);
      buffer->set_attrib_pointer<float>(0, 3, 0, 0, 0);
      buffer->vertex_count = vertices.size();
      return buffer;
   }

}