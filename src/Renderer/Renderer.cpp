#include "Renderer.hpp"

namespace Synth3D
{
   void Renderer::render_triangles(GeometryBuffer &buffer)
   {
      glBindVertexArray(buffer.vao);
      glDrawArrays(GL_TRIANGLES, 0, buffer.vertex_count);
      glBindVertexArray(0);
   }
   void Renderer::render_points(GeometryBuffer &buffer)
   {
      glBindVertexArray(buffer.vao);
      glDrawArrays(GL_POINTS, 0, buffer.vertex_count);
      glBindVertexArray(0);
   }
   void Renderer::render_lines(GeometryBuffer &buffer, int start, int size)
   {
      glBindVertexArray(buffer.vao);
      glDrawArrays(GL_LINES, start, size);
      glBindVertexArray(0);
   }
   void Renderer::enable_blend_test()
   {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }

   void Renderer::enable_cull_face()
   {
      glEnable(GL_CULL_FACE);
   }

   void Renderer::enable_depth_test()
   {

      glEnable(GL_DEPTH_TEST);
   }
   void Renderer::disable_depth_test()
   {
      glDisable(GL_DEPTH_TEST);
   }
   void Renderer::enable_stencil_test()
   {
      glEnable(GL_STENCIL_TEST);
   }

   void Renderer::enable_multisample()
   {
      glEnable(GL_MULTISAMPLE);
   }
   void Renderer::disable_stencil_test()
   {
      glDisable(GL_STENCIL_TEST);
   }
   void Renderer::clear_buffer()
   {
      glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   }
   void Renderer::set_viewport(int x, int y, int width, int height)
   {
      glViewport(x, y, width, height);
   }
}