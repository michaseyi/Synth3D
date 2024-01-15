#include "Shader.hpp"
#include "../Utils/Utils.hpp"

namespace Synth3D
{
   /**
    * @throws std::runtime_error if it fails to create a shader or link the shader program
    */
   Shader::Shader(std::string &vertex_shader_source, std::string &fragment_shader_source)
   {
      GLuint vertex_shader = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
      GLuint fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

      GLuint shader_program = create_program(vertex_shader, fragment_shader);
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
      id = shader_program;
   }

   /**
    * @throws std::runtime_error if it fails to create a shader or link the shader program
    */
   Shader::Shader(std::string &vertex_shader_source, std::string &fragment_shader_source, std::string &geometry_shader_source)
   {
      GLuint vertex_shader = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
      GLuint fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
      GLuint geometric_shader = compile_shader(geometry_shader_source, GL_GEOMETRY_SHADER);

      GLuint shader_program = create_program(vertex_shader, fragment_shader, geometric_shader);
      glDeleteShader(vertex_shader);
      glDeleteShader(fragment_shader);
      id = shader_program;
   }
   /**
    * @brief  throws runtime error if shader linkage fails
    *
    */
   template <typename... T>
   GLuint Shader::create_program(T... shaders)
   {
      GLuint program = glCreateProgram();
      GLint success = 0;
      char info_log[512];

      (glAttachShader(program, shaders), ...);
      glLinkProgram(program);
      glGetProgramiv(program, GL_LINK_STATUS, &success);
      if (!success)
      {
         (glDeleteShader(shaders), ...);
         glGetProgramInfoLog(program, 512, nullptr, info_log);
         LOG_ERROR(info_log);
         throw std::runtime_error(std::string("Failed to link shader program: ") + info_log);
      }
      return program;
   }

   /**
    * @brief throws runtime error if shader compilation fails
    *
    */
   GLuint Shader::compile_shader(std::string &source, GLenum shader_type)
   {
      GLuint shader_id = glCreateShader(shader_type);
      const char *shader_cstr = source.c_str();

      glShaderSource(shader_id, 1, &shader_cstr, nullptr);
      glCompileShader(shader_id);

      GLint success = 0;
      char info_log[512];
      glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
      if (!success)
      {
         glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
         LOG_ERROR(info_log);
         throw std::runtime_error(std::string("Failed to compile shader: ") + info_log);
      }
      return shader_id;
   }
   Shader::Shader(Shader &&other_shader)
   {
      id = other_shader.id;
      other_shader.id = 0;
   }
   Shader::~Shader()
   {
      if (id)
      {
         glDeleteProgram(id);
      }
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

   void Shader::bind()
   {
      glUseProgram(id);
   }
   void Shader::unbind()
   {
      glUseProgram(0);
   }
   void Shader::set_vec3(std::string uniform_name, glm::vec3 value)
   {
      glUniform3fv(get_uniform_location(uniform_name), 1, glm::value_ptr(value));
   }
   void Shader::set_vec2(std::string uniform_name, glm::vec2 value)
   {
      glUniform2fv(get_uniform_location(uniform_name), 1, glm::value_ptr(value));
   }

}