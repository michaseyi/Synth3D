#include "Buffer.hpp"
#include "../Utils/Utils.hpp"

namespace Synth3D
{
#ifndef BUFFER_TEMPLATE_IMPLEMENTATION
   GeometryBuffer::GeometryBuffer()
   {
      glGenVertexArrays(1, &vao);
   }

   void GeometryBuffer::create_vbos(int count)
   {
      vbos.resize(count);
      glGenBuffers(count, vbos.data());
   }

   GeometryBuffer::GeometryBuffer(GeometryBuffer &&other_buffer)
   {
      vao = other_buffer.vao;
      vbos = std::move(other_buffer.vbos);

      other_buffer.vao = 0;
      other_buffer.vbos.clear();
   }
   GeometryBuffer::~GeometryBuffer()
   {
      if (vao)
      {
         glDeleteVertexArrays(1, &vao);
      }

      glDeleteBuffers(vbos.size(), vbos.data());
   }

#else
   template <typename T>
   void GeometryBuffer::buffer_data(T *source, uint size, int buffer_index)
   {
      if (!(buffer_index >= 0 && buffer_index < vbos.size()))
      {
         LOG_ERROR("Invalid buffer index");
         throw std::runtime_error("Invalid buffer index");
      }
      glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER, vbos[buffer_index]);

      glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), source, GL_DYNAMIC_DRAW);

      glBindVertexArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
   }

   template <typename T>
   void GeometryBuffer::set_attrib_pointer(uint location, uint length, uint stride, uintptr_t offset, int buffer_index)
   {
      if (!(buffer_index >= 0 && buffer_index < vbos.size()))
      {
         LOG_ERROR("Invalid buffer index");
         throw std::runtime_error("Invalid buffer index");
      }
      glBindVertexArray(vao);
      glBindBuffer(GL_ARRAY_BUFFER, vbos[buffer_index]);

      glVertexAttribPointer(location, length, buffer_datatype_map[std::type_index(typeid(T))], GL_FALSE, stride, (void *)offset);
      glEnableVertexAttribArray(location);

      glBindVertexArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
   }

#endif
}