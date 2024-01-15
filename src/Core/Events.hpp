#pragma once
#include <functional>

namespace Synth3D
{

   template <typename T>
   using EventCallback = std::function<void(T &)> *;

   template <typename T>
   class EventManager
   {
   public:
      template <typename U>
      void add_listener();

      void template <typename U>
      void remove_listener();

      template <typename U>
      void dispatch();

   private:
      std::unordered_map<std::type_index, std::unordered_set<int>> listeners;
   };
}