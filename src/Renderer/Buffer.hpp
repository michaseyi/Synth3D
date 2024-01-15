#pragma once
#include <iostream>
#include <vector>
#include "../../external/glad/glad.h"
#include <unordered_map>
#include <typeindex>

namespace Synth3D
{
   using uint = unsigned int;
   class GeometryBuffer
   {
   public:
      GeometryBuffer();
      GeometryBuffer(const GeometryBuffer &) = delete;
      GeometryBuffer(GeometryBuffer &&other_buffer);
      ~GeometryBuffer();
      uint vertex_count = 0;
      void create_vbos(int count);
      template <typename T>
      void buffer_data(T *source, uint size, int buffer_index);

      template <typename T>
      void set_attrib_pointer(uint location, uint length, uint stride, uintptr_t offset, int buffer_index);

      GLuint vao;
      std::vector<GLuint> vbos;

      inline static std::unordered_map<std::type_index, GLenum> buffer_datatype_map = {
          std::make_pair(std::type_index(typeid(float)), GL_FLOAT),
          std::make_pair(std::type_index(typeid(unsigned int)), GL_UNSIGNED_INT),
      };
   };
}

#define BUFFER_TEMPLATE_IMPLEMENTATION
#include "Buffer.cpp"
#undef BUFFER_TEMPLATE_IMPLEMENTATION