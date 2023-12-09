#include "Shader.hpp"
#include "../Utils/Utils.hpp"

namespace Synth3D
{
   /**
    * @throws std::runtime_error if it fails to create a shader or link the shader program
    */
   Shader::Shader(std::string &vertex_shader_source, std::string &fragment_shader_source)
   {
      std::cout << "ns constructor" << std::endl;
      GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      const char *vertex_shader_cstr = vertex_shader_source.c_str();

      glShaderSource(vertex_shader, 1, &vertex_shader_cstr, nullptr);
      glCompileShader(vertex_shader);

      GLint success;
      char info_log[512];
      glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
         glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
         LOG_ERROR(info_log);
         throw std::runtime_error(std::string("Failed to compile vertex shader: ") + info_log);
      }

      GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
      const char *fragment_shader_cstr = fragment_shader_source.c_str();
      glShaderSource(fragment_shader, 1, &fragment_shader_cstr, nullptr);

      glCompileShader(fragment_shader);
      glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

      if (!success)
      {
         glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
         LOG_ERROR(info_log);
         throw std::runtime_error(std::string("Failed to compile fragment shader: ") + info_log);
      }

      GLuint shader_program = glCreateProgram();
      glAttachShader(shader_program, vertex_shader);
      glAttachShader(shader_program, fragment_shader);
      glLinkProgram(shader_program);

      glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
      if (!success)
      {
         glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
         LOG_ERROR(info_log);
         throw std::runtime_error(std::string("Failed to link shader program: ") + info_log);
      }

      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
      id = shader_program;

      addr = malloc(10);
   }

   Shader::~Shader()
   {
      std::cout << "ns" << id << std::endl;
      free(addr);
      glDeleteProgram(id);
   }
   void Shader::use()
   {
      glUseProgram(id);
   }
   GLint Shader::get_uniform_location(std::string uniform_name)
   {
      return glGetUniformLocation(id, uniform_name.c_str());
   }

   void Shader::set_bool(std::string uniform_name, bool value)
   {
      glUniform1i(get_uniform_location(uniform_name), value);
   }
   void Shader::set_int(std::string uniform_name, int value)
   {
      glUniform1i(get_uniform_location(uniform_name), value);
   }
   void Shader::set_float(std::string uniform_name, float value)
   {
      glUniform1f(get_uniform_location(uniform_name), value);
   }
   void Shader::set_mat4(std::string uniform_name, glm::mat4 value)
   {
      glUniformMatrix4fv(get_uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(value));
   }
   void Shader::set_vec3(std::string uniform_name, glm::vec3 value)
   {
      glUniform3fv(get_uniform_location(uniform_name), 1, glm::value_ptr(value));
   }
   void Shader::set_vec2(std::string uniform_name, glm::vec3 value)
   {
      glUniform2fv(get_uniform_location(uniform_name), 1, glm::value_ptr(value));
   }

   std::string basic_material_vertex_shader = R"(
      #version 430 core
      
      void main() {
         gl_Position = vec4(1.0);
      }
   )";
   std::string basic_material_fragment_shader = R"(
      #version 430 core

      out vec4 FragColor;

      void main() {
         FragColor = vec4(1.0);
      }
   )";
   Shader basic_material(glm::vec3 color)
   {
      Shader shader(basic_material_vertex_shader, basic_material_fragment_shader);
      shader.set_vec3("uColor", color);
      return shader;
   }
}