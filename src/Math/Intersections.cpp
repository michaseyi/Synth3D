#include "Intersections.hpp"
#include <iostream>

namespace Synth3D
{

   glm::vec3 compute_ray_from_screen_space(float xpos, float ypos, glm::vec2 &screen_size, glm::mat4 &view_matrix, glm::mat4 &projection_matrix)
   {
      glm::mat4 inverse_matrix = glm::inverse(projection_matrix * view_matrix);

      float half_screen_width = screen_size.x / 2;
      float half_screen_height = screen_size.y / 2;

      glm::vec4 near = glm::vec4((xpos - half_screen_width) / half_screen_width, -1 * (ypos - half_screen_height) / half_screen_height, -1.0f, 1.0f);
      glm::vec4 far = glm::vec4((xpos - half_screen_width) / half_screen_width, -1 * (ypos - half_screen_height) / half_screen_height, 1.0f, 1.0f);

      near = inverse_matrix * near;
      far = inverse_matrix * far;

      near /= near.w;
      far /= far.w;

      return glm::normalize(far - near);
   }
   bool ray_intersects_sphere(glm::vec3 &ray_direction, glm::vec3 &ray_origin, glm::vec3 &sphere_radius, glm::vec3 &sphere_position)
   {
      glm::vec3 oc = ray_origin - sphere_position;
      float a = glm::dot(ray_direction, ray_direction);
      float b = 2.0f * glm::dot(oc, ray_direction);
      float c = glm::dot(oc, oc) - (sphere_radius.x * sphere_radius.x);
      float discriminant = b * b - 4 * a * c;
      return discriminant >= 0;
   }
}