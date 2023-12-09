#pragma once
#include <string>

#define LOG_ERROR(message)                                                 \
   do                                                                      \
   {                                                                       \
      std::cerr << "File: " << __FILE__ << ", Function: " << __func__      \
                << ", Line: " << __LINE__ << ": " << message << std::endl; \
   } while (0)

namespace Synth3D
{
   void load_image(std::string image_path);
}