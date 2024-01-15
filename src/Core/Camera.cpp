#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
namespace Synth3D
{
   Camera::Camera(glm::vec3 position, glm::vec3 reference)
   {
      this->distance = glm::length(position - reference);
      this->position = position;
      this->reference = reference;
      glm::vec3 front = glm::normalize(reference - position);
      glm::vec3 x(1.0f, 0.0f, 0.0f);
      float rotation_angle = glm::acos(glm::dot(front, x));
      glm::vec3 rotation_axis = glm::normalize(glm::cross(x, front));
      orientation = glm::normalize(glm::angleAxis(rotation_angle, rotation_axis));
   }

   Camera::Camera() : Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f)) {}

   glm::mat4 Camera::view()
   {
      glm::vec3 up = orientation * glm::vec3(0, 1.0, 0);
      return glm::lookAt(position, reference, up);
   }
   void Camera::zoom(float factor)
   {
      distance += factor;
      distance = distance < 0 ? 0 : distance;
      position = reference + (distance * -(orientation * glm::vec3(1.0, 0.0, 0.0)));
   }
   glm::mat4 Camera::projection(float aspect_ratio)
   {
      return glm::perspective(glm::radians(40.0f), aspect_ratio, 0.1f, 100.0f);
   }

   void Camera::multiply_quat(glm::quat rotation)
   {
      orientation = glm::normalize(rotation * orientation);
      position = reference + (distance * -(orientation * glm::vec3(1.0, 0.0, 0.0)));
   }
}