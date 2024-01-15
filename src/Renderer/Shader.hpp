#pragma once
#include <iostream>
#include <memory>
#include "../../external/glad/glad.h"
#include <GLFW/glfw3.h>
#include "../Math/glm.hpp"

namespace Synth3D
{
   class Shader
   {
   public:
      Shader() = default;
      Shader(std::string &vertex_shader, std::string &fragment_shader);
      Shader(std::string &vertex_shader, std::string &fragment_shader, std::string &geometry_shader);
      Shader(Shader &&);
      ~Shader();
      GLint get_uniform_location(std::string uniform_name);
      void set_bool(std::string name, bool value);
      void set_int(std::string name, int value);
      void set_float(std::string name, float value);
      void set_mat4(std::string name, glm::mat4 value);
      void set_vec3(std::string name, glm::vec3 value);
      void set_vec2(std::string name, glm::vec2 value);
      GLuint compile_shader(std::string &source, GLenum shader_type);
      template <typename... T>
      GLuint create_program(T... shaders);
      void bind();
      void unbind();
      GLuint id = 0;
   };

}