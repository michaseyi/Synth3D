#pragma once
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include "../Renderer/Shader.hpp"

namespace Synth3D
{
   typedef OpenMesh::TriMesh_ArrayKernelT<> Geometry;
   class Mesh
   {
   public:
      Geometry geometry;
      Shader material;
   };

}


// Geometry cube;
//    Geometry::VertexHandle vhandle[4];

//    vhandle[0] = cube.add_vertex(Geometry::Point(-1, -1, 0));
//    vhandle[1] = cube.add_vertex(Geometry::Point(1, -1, 0));
//    vhandle[2] = cube.add_vertex(Geometry::Point(1, 1, 0));
//    vhandle[3] = cube.add_vertex(Geometry::Point(-1, 1, 0));

//    std::vector<Geometry::VertexHandle> face_vhandles;
//    face_vhandles.push_back(vhandle[0]);
//    face_vhandles.push_back(vhandle[1]);
//    face_vhandles.push_back(vhandle[2]);

//    cube.add_face(face_vhandles);
//    face_vhandles.clear();

//    face_vhandles.push_back(vhandle[0]);
//    face_vhandles.push_back(vhandle[2]);
//    face_vhandles.push_back(vhandle[3]);
//    cube.add_face(face_vhandles);

//    Geometry::VertexIter v_it, v_end = cube.vertices_end();

//    for (v_it = cube.vertices_begin(); v_it != v_end; v_it++)
//    {
//       std::cout << cube.point(*v_it) << std::endl;
//    }

//    std::cout << "\n";

//    for (auto face : cube.faces())
//    {
//       auto normal = cube.calc_face_normal(face);
//       std::cout << normal << std::endl;
//    }