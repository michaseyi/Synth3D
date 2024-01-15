#pragma once
#include "glm.hpp"

namespace Synth3D
{
   glm::vec3 compute_ray_from_screen_space(float xpos, float ypos, glm::vec2 &screen_size, glm::mat4 &view_matrix, glm::mat4 &projection_matrix);

   bool ray_intersects_sphere(glm::vec3 &ray_direction, glm::vec3 &ray_origin, glm::vec3 &sphere_radius, glm::vec3 &sphere_position);
}