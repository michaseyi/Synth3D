#pragma once
#include "../Math/glm.hpp"
#include <algorithm>
#include "Window.hpp"
namespace Synth3D
{
   class Camera
   {
   public:
      Camera(glm::vec3 position, glm::vec3 reference);
      Camera();
      glm::vec3 position;
      glm::mat4 view();
      glm::mat4 projection(float aspect_ratio);
      void multiply_quat(glm::quat rotation);
      void zoom(float factor);
      std::function<void(const WindowEvent *e)> mouse_scroll_callback = [this](const WindowEvent *e)
      {
         auto event = reinterpret_cast<const MouseScrollEvent *>(e);
         zoom(-event->yoffset / 3.0f);
      };
      std::function<void(const WindowEvent *e)> mouse_down_callback = [this](const WindowEvent *e)
      {
         auto event = reinterpret_cast<const PointerDownEvent *>(e);
         prev_mouse_pos[0] = event->xpos;
         prev_mouse_pos[1] = event->ypos;
         mouse_moving = true;
      };
      std::function<void(const WindowEvent *e)> mouse_move_callback = [this](const WindowEvent *e)
      {
         if (mouse_moving)
         {
            auto event = reinterpret_cast<const PointerMoveEvent *>(e);
            glm::vec2 diff{prev_mouse_pos[0] - event->xpos, prev_mouse_pos[1] - event->ypos};
            multiply_quat(glm::angleAxis(diff.x / 100.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
            multiply_quat(glm::angleAxis(diff.y / 100.0f, orientation * glm::vec3(0.0f, 0.0f, 1.0f)));
            prev_mouse_pos[0] = event->xpos;
            prev_mouse_pos[1] = event->ypos;
         }
      };
      std::function<void(const WindowEvent *e)> mouse_up_callback = [this](const WindowEvent *e)
      {
         auto event = reinterpret_cast<const PointerUpEvent *>(e);
         mouse_moving = false;
      };

   private:
      double prev_mouse_pos[2];
      bool mouse_moving = false;
      glm::vec3 reference;
      glm::quat orientation;
      float distance;
   };

}