#pragma once
#include "Components.hpp"
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <algorithm>

namespace Synth3D
{

   class Node
   {
   public:
      Node(std::string name);
      ~Node();
      void add_child(Node *);
      void remove_child(Node *);
      template <typename T>
      void remove_component();
      void add_component(Component *component);
      template <typename T>
      T *get_component();
      template <typename... Args>
      bool get_multiple_components(Args *&...);
      template <typename... Args>
      bool has_components();
      template <typename T>
      bool has_component();
      template <typename T>
      void set(T *&component_receiver);
      std::string name;
      Node *parent = nullptr;

      std::vector<Node *> child_nodes;

   private:
      std::unordered_map<std::type_index, Component *> components;
   };

   class SceneGraph
   {
   public:
      SceneGraph();
      SceneGraph(SceneGraph &&);
      void clean_up();
      void add(Node *);
      Node *root;
      Node *active_node = nullptr;
   };

}
#define SCENE_TEMPLATE_IMPLEMENTATION
#include "Scene.cpp"
#undef SCENE_TEMPLATE_IMPLEMENTATION