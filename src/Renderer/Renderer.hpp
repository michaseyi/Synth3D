#pragma once
#include "../Core/Mesh.hpp"

namespace Synth3D
{
   class Renderer
   {
   public:
      void render(Mesh &mesh);
      void enable_depth_test();
      void disable_depth_test();
      void enable_stencil_test();
      void disable_stencil_test();
      void clear_buffer();
      void set_view_port(int x, int y, int width, int height);
   };
}
