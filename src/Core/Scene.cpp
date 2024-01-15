#include "Scene.hpp"

namespace Synth3D
{

#ifndef SCENE_TEMPLATE_IMPLEMENTATION
   Node::Node(std::string name) : name(name) {}

   Node::~Node()
   {
      for (auto i = components.begin(); i != components.end(); i++)
      {
         delete i->second;
      }

      for (auto child : child_nodes)
      {
         delete child;
      }
   }
   void Node::add_child(Node *child)
   {
      child->parent = this;
      child_nodes.emplace_back(child);
   }

   void Node::remove_child(Node *child)
   {
      for (auto i = child_nodes.begin(); i != child_nodes.end(); i++)
      {
         if ((*i) == child)
         {
            child_nodes.erase(i);
            break;
         }
      }
   }

   void Node::add_component(Component *component)
   {
      auto index = std::type_index(typeid(*component));
      components[index] = component;
   }

   SceneGraph::SceneGraph()
   {
      root = new Node("Scene");
   }

   SceneGraph::SceneGraph(SceneGraph &&other_scene)
   {
      root = other_scene.root;
      other_scene.root = nullptr;
   }

   void SceneGraph::add(Node *node)
   {
      root->add_child(node);
   }

   void SceneGraph::clean_up()
   {
      if (root)
      {
         delete root;
         root = nullptr;
      }
   }

#else
   template <typename T>
   void Node::remove_component()
   {
      static_assert(std::is_base_of<Component, T>::value, "Not a valid component");
      auto component = get_component<T>();

      if (component)
      {
         components.erase(std::type_index(typeid(T)));
         delete component;
      }
   }

   template <typename T>
   T *Node::get_component()
   {
      static_assert(std::is_base_of<Component, T>::value, "Not a valid component");
      return dynamic_cast<T *>(components[std::type_index(typeid(T))]);
   }

   template <typename T>
   void Node::set(T *&component_receiver)
   {
      component_receiver = get_component<T>();
   }

   template <typename... Args>
   bool Node::get_multiple_components(Args *&...component)
   {
      auto has_all_required_components = has_components<Args...>();
      if (has_all_required_components)
      {
         (set<Args>(component), ...);
         return true;
      }
      return false;
   }

   template <typename... Args>
   bool Node::has_components()
   {
      return (has_component<Args>() && ...);
   }
   template <typename T>
   bool Node::has_component()
   {
      return get_component<T>() != nullptr;
   }

#endif
}
