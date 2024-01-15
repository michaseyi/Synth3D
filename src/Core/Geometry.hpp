#pragma once
#include "Runtime.hpp"
#include "Mesh.hpp"
#include "../Renderer/Buffer.hpp"

namespace Synth3D
{
   void add_cube(MeshData &mesh_data);
   void add_sphere(MeshData &mesh_data, float radius, int latitude_segments, int longitude_segments);
   void add_knot(MeshData &mesh_data, float radius, float thickness, int num_segments, int num_slices);
   void add_cylinder(MeshData &mesh_data, float radius, float height, int num_segments);
   GeometryBuffer *create_helper_grid(float size, int divisions);

}