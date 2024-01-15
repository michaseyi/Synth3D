#include "Window.hpp"

namespace Synth3D
{
#ifndef WINDOW_TEMPLATE_IMPLEMENTATION
   Window::Window(std::string name, int width, int height) : name(name)
   {

      const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
      height = mode->height;
      width = mode->width;

      handle = glfwCreateWindow(mode->width, mode->height, name.c_str(), nullptr, nullptr);
      if (handle == nullptr)
      {
         LOG_ERROR("Failed to create window.");
         throw std::runtime_error("Failed to create Window.");
      }
      glfwMakeContextCurrent(handle);
      active_windows[handle] = this;

      register_event_handlers();
   }

   Window::~Window()
   {
      active_windows.erase(handle);
      glfwDestroyWindow(handle);
   }

   void Window::register_event_handlers()
   {
      glfwSetMouseButtonCallback(handle, [](GLFWwindow *window, int button, int action, int mods)
                                 {
         auto window_iterator = Window::active_windows.find(window);
         if (window_iterator != Window::active_windows.end()) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            if (action == GLFW_PRESS) {
               PointerDownEvent event{xpos,ypos};
               window_iterator->second->dispatch(&event);
            } else {
               PointerUpEvent event{xpos, ypos};
               window_iterator->second->dispatch(&event);
            }
         } });

      glfwSetCursorPosCallback(handle, [](GLFWwindow *window, double xpos, double ypos)
                               {
         
         auto window_iterator = Window::active_windows.find(window);
         if (window_iterator != Window::active_windows.end())
         {
            PointerMoveEvent event{xpos, ypos};
            window_iterator->second->dispatch(&event);
         } });

      glfwSetScrollCallback(handle, [](GLFWwindow *window, double xoffset, double yoffset)
                            {
         auto window_iterator = Window::active_windows.find(window);
         if (window_iterator != Window::active_windows.end())
         {
            MouseScrollEvent event{xoffset, yoffset};
            window_iterator->second->dispatch(&event);
         } });

      glfwSetFramebufferSizeCallback(handle, [](GLFWwindow *window, int width, int height)
                                     {
         auto window_iterator = Window::active_windows.find(window);
         if (window_iterator != Window::active_windows.end()) {
            WindowResizeEvent event{width, height};
            window_iterator->second->framebuffer_resize_callback(&event);
            window_iterator->second->dispatch(&event);
         } });
   }
   void Window::poll_events()
   {
      glfwPollEvents();
   }

   bool Window::is_open()
   {
      return !glfwWindowShouldClose(handle);
   }

   GLFWwindow *Window::window_handle()
   {
      return handle;
   }

   glm::vec2 Window::window_size()
   {
      return glm::vec2{width, height};
   }
#else

   template <typename T>
   void Window::dispatch(T *event)
   {
      static_assert(std::is_base_of<WindowEvent, T>::value, "Not a valid event");
      std::type_index index = std::type_index(typeid(T));
      auto callbacks = listeners.find(index);
      if (callbacks != listeners.end())
      {
         for (auto callback : callbacks->second)
         {
            (*callback)(event);
         }
      }
   }

   template <typename T>
   void Window::add_listener(WindowEventCallback listener)
   {
      static_assert(std::is_base_of<WindowEvent, T>::value, "Not a valid event");
      std::type_index index = std::type_index(typeid(T));
      auto event_callbacks = listeners.find(index);
      if (event_callbacks == listeners.end())
      {
         listeners[index] = std::unordered_set<WindowEventCallback>();
         event_callbacks = listeners.find(index);
      }
      event_callbacks->second.emplace(listener);
   }

   template <typename T>
   void Window::remove_listener(WindowEventCallback listener)
   {
      static_assert(std::is_base_of<WindowEvent, T>::value, "Not a valid event");
      std::type_index index = std::type_index(typeid(T));
      auto event_callbacks = listeners.find(index);
      if (event_callbacks != listeners.end())
      {
         event_callbacks->second.erase(listener);
      }
   }
#endif
}