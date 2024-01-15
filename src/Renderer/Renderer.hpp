#pragma once
#include "Buffer.hpp"
#include "../../external/glad/glad.h"
namespace Synth3D
{
   class Renderer
   {
   public:
      void render_triangles(GeometryBuffer &buffer);
      void render_points(GeometryBuffer &buffer);
      void render_lines(GeometryBuffer &buffer, int start, int size);
      void enable_multisample();
      void enable_blend_test();
      void enable_depth_test();
      void disable_depth_test();
      void enable_stencil_test();
      void disable_stencil_test();
      void enable_cull_face();
      void clear_buffer();
      void set_viewport(int x, int y, int width, int height);
   };
}
