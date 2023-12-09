#pragma once
#include <iostream>
#include "../../external/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../Utils/Utils.hpp"
namespace Synth3D
{
   struct Window
   {
      GLFWwindow *window_handle;
      int width;
      int height;
      std::string name;
   };

   class Runtime
   {
   public:
      Runtime(std::string window_name, int window_width, int window_height);
      ~Runtime();
      int window_width;
      int window_height;
      std::string window_name;
      GLFWwindow *active_window;
      void clean_up();
      void poll_events();

      static void main_loop();
   };

}

extern Synth3D::Runtime runtime;