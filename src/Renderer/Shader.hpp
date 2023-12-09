#pragma once
#include <iostream>

#include "../../external/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Synth3D
{
   class Shader
   {
   public:
      Shader(std::string &vertex_shader, std::string &fragment_shader);
      ~Shader();
      void *addr;
      void use();
      GLint get_uniform_location(std::string uniform_name);
      void set_bool(std::string name, bool value);
      void set_int(std::string name, int value);
      void set_float(std::string name, float value);
      void set_mat4(std::string name, glm::mat4 value);
      void set_vec3(std::string name, glm::vec3 value);
      void set_vec2(std::string name, glm::vec3 value);

   private:
      GLuint id;
   };

   Shader basic_material(glm::vec3 color);
}