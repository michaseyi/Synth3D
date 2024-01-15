#pragma once
#include <cassert>
#include <sstream>
#include <string>
#define ASSERT(condition, message)                                             \
   do                                                                          \
   {                                                                           \
      if (!(condition))                                                        \
      {                                                                        \
         std::ostringstream os;                                                \
         os << "Assertion failed: (" #condition "), function " << __FUNCTION__ \
            << ", file " << __FILE__ << ", line " << __LINE__ << ". "          \
            << message;                                                        \
         throw std::runtime_error(os.str());                                   \
      }                                                                        \
   } while (0)

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
