#pragma once
#include <iostream>
#include "../../external/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../Utils/Utils.hpp"
#include "Scene.hpp"
#include "../Renderer/Renderer.hpp"
#include "Camera.hpp"
#include "Window.hpp"

namespace Synth3D
{
   class Runtime
   {
   public:
      ~Runtime();
      Runtime(const Runtime &rhs) = delete;
      void operator=(const Runtime &rhs) = delete;
      bool is_open();
      void update();
      SceneGraph scene;
      static Runtime &get_instance();
      // callbacks
      std::function<void(const WindowEvent *)> mosue_down_callback = [this](const WindowEvent *e)
      {
         auto event = reinterpret_cast<const PointerDownEvent *>(e);
         compute_mouse_click_intersections_with_world(event->xpos, event->ypos);
      };


   private:
      Shader *default_shader;
      Shader *highlight_shader;
      Shader *grid_shader;
      GeometryBuffer *grid_buffer;
      Camera editor_camera;
      Window *window;
      Renderer renderer;
      void swap_buffers();
      void render_current_scene();
      void compute_mouse_click_intersections_with_world(double mouse_x_position, double mouse_y_position);
      Runtime(std::string window_name, int window_width, int window_height);
   };

}
