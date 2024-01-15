#pragma once
#include <iostream>
#include <exception>
#include "../Utils/Utils.hpp"
#include "../../external/glad/glad.h"
#include "../Math/glm.hpp"
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <functional>

namespace Synth3D
{

   struct WindowEvent
   {
   };

   struct PointerMoveEvent : public WindowEvent
   {
      PointerMoveEvent(double xpos, double ypos) : xpos(xpos), ypos(ypos) {}
      double xpos;
      double ypos;
   };

   struct MouseScrollEvent : public WindowEvent
   {
      MouseScrollEvent(double xoffset, double yoffset) : xoffset(xoffset), yoffset(yoffset) {}
      double xoffset;
      double yoffset;
   };
   struct WindowResizeEvent : public WindowEvent
   {
      WindowResizeEvent(int width, int height) : width(width), height(height) {}
      int width;
      int height;
   };

   struct PointerDownEvent : public WindowEvent
   {
      PointerDownEvent(double xpos, double ypos) : xpos(xpos), ypos(ypos) {}
      double xpos;
      double ypos;
   };
   struct PointerUpEvent : public WindowEvent
   {
      PointerUpEvent(double xpos, double ypos) : xpos(xpos), ypos(ypos) {}
      double xpos;
      double ypos;
   };

   using WindowEventCallback = std::function<void(const WindowEvent *)> *;

   class Window
   {
   public:
      Window(std::string name, int width, int height);
      ~Window();
      void poll_events();
      bool is_open();
      template <typename T>
      void add_listener(WindowEventCallback listener);
      template <typename T>
      void remove_listener(WindowEventCallback listener);
      template <typename T>
      void dispatch(T *event);
      GLFWwindow *window_handle();
      static inline std::unordered_map<GLFWwindow *, Window *> active_windows{};
      glm::vec2 window_size();

      std::function<void(const WindowEvent *)> framebuffer_resize_callback = [this](const WindowEvent *e)
      {
         auto event = reinterpret_cast<const WindowResizeEvent *>(e);
         width = event->width;
         height = event->height;
      };

   private:
      GLFWwindow *handle;
      int width;
      int height;
      std::string name;
      std::unordered_map<std::type_index, std::unordered_set<WindowEventCallback>> listeners;

      void register_event_handlers();
   };

}
#define WINDOW_TEMPLATE_IMPLEMENTATION
#include "Window.cpp"
#undef WINDOW_TEMPLATE_IMPLEMENTATION