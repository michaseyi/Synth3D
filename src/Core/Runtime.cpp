#include "Runtime.hpp"

namespace Synth3D
{
   /**
    * @throws std::runtime_error if glfw init, window creation, glad init fails
    */
   Runtime::Runtime(std::string window_name, int window_width, int window_height)
       : window_height(window_height), window_width(window_width), window_name(window_name)
   {
      if (glfwInit() == GL_FALSE)
      {
         LOG_ERROR("Failed to initialize GLFW");
         throw std::runtime_error("Failed to initialize GLFW.");
      }

#ifdef EMSCRIPTEN
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
      glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

      GLFWwindow *window = glfwCreateWindow(window_width, window_height, window_name.c_str(), nullptr, nullptr);
      if (window == nullptr)
      {
         LOG_ERROR("Failed to create window.");
         throw std::runtime_error("Failed to create window.");
      }
      active_window = window;
      glfwMakeContextCurrent(active_window);
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      {
         LOG_ERROR("Failed to initialize glad.");
         throw std::runtime_error("Failed to initialize glad.");
      }
   }

   void Runtime::clean_up()
   {
      glfwTerminate();
      glfwDestroyWindow(active_window);
   }
   void Runtime::poll_events()
   {
      glfwPollEvents();
   }
   Runtime::~Runtime()
   {
      clean_up();
   }

   void Runtime::main_loop()
   {
      while (!glfwWindowShouldClose(runtime.active_window))
      {
         runtime.poll_events();
      }
   }

}
