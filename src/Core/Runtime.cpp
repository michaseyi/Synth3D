#include "Runtime.hpp"
#include "Geometry.hpp"
#include "../Math/Intersections.hpp"

std::string basic_material_vertex_shader = R"glsl(
      #version 430 core

      layout (location = 0) in vec3 aPosition;
      layout (location = 2) in vec3 aNormal;


      uniform mat4 ModelMatrix;
      uniform mat4 ViewMatrix;
      uniform mat4 ProjectionMatrix;

      out vec3 Normal;
      out vec3 Position;

      void main() {
         Position = (ModelMatrix * vec4(aPosition, 1.0)).xyz;
         Normal = normalize(mat3(transpose(inverse(ModelMatrix))) * aNormal);
 
         gl_Position = ProjectionMatrix * ViewMatrix * vec4(Position, 1.0);
      }
   )glsl";

std::string basic_material_fragment_shader = R"glsl(
      #version 430 core
      in vec3 Normal;
      in vec3 Position;
      uniform mat4 Color;

      out vec4 FragColor;

      uniform vec3 LightPosition;
      uniform vec3 CameraPosition;

      void main() {

         vec3 color = vec3(0.8, 0.8, 0.8);
         // color = vec3(0.0, 1.0, 0.0);
         vec3 ambient = vec3(0.3);

         vec3 lightVector = normalize(LightPosition - Position);
         vec3 diffuse = vec3(0.5) * max(0, dot(lightVector, Normal));
         vec3 viewDir = normalize(CameraPosition - Position);
         float spec = pow(max(dot(viewDir, reflect(-lightVector, Normal)), 0.0), 200);
         vec3 specular = 0.8 * spec * vec3(1.0);
         
         color *= diffuse + ambient + specular;
         FragColor = vec4(color, 1.0);
        
      }
   )glsl";

std::string highlight_vertex_shader = R"glsl(
      #version 430 core

      layout (location = 0) in vec3 aPosition;
      layout (location = 2) in vec3 aNormal;
      uniform mat4 ModelMatrix;
      uniform mat4 ViewMatrix;
      uniform mat4 ProjectionMatrix;
      uniform vec2 ScreenDimension;
      

      void main() {
         vec4 position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(aPosition, 1.0);
         vec3 normal = normalize(mat3(transpose(inverse(ProjectionMatrix * ViewMatrix * ModelMatrix))) * aNormal);
         position.xy += normalize(normal.xy) / ScreenDimension * 2 * position.w * 2;
         
         gl_Position = position;
      }
   )glsl";

std::string highlight_fragment_shader = R"glsl(
      #version 430 core

      uniform vec3 Color;

      out vec4 FragColor;

      void main() {

         FragColor = vec4(Color, 1.0);
        
      }
   )glsl";
std::string grid_vertex_shader = R"glsl(
      #version 430 core

      layout (location = 0) in vec3 aPosition;
      uniform mat4 ViewMatrix;
      uniform mat4 ProjectionMatrix;
      
      out vec3 Position;
      void main() {
         Position = aPosition; 
         gl_Position = ProjectionMatrix * ViewMatrix * vec4(aPosition, 1.0);
      }
   )glsl";

std::string grid_fragment_shader = R"glsl(
      #version 430 core

      in vec3 Position;

      uniform  vec3 CameraPosition;

      uniform vec3 Color;

      out vec4 FragColor;

      void main() {
         vec3 eyeVector = normalize(CameraPosition - Position);
         vec3 surfaceNormal = vec3(0.0, 1.0, 0.0); 
         float cosTheta = abs(dot(surfaceNormal, eyeVector));
         float alpha = smoothstep(0.0, 1.0, cosTheta);
        
         vec3 color = Color;
         
         FragColor = vec4(color, alpha);
        
      }
   )glsl";

namespace Synth3D
{
   /**
    * @throws std::runtime_error if glfw init, window creation, glad init fails
    */
   Runtime::Runtime(std::string window_name, int window_width, int window_height)
   {
      if (glfwInit() == GL_FALSE)
      {
         LOG_ERROR("Failed to initialize GLFW");
         throw std::runtime_error("Failed to initialize GLFW.");
      }

#ifdef EMSCRIPTEN
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
      glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_SAMPLES, 8);
#endif

      window = new Window(window_name, window_width, window_height);

      // register camera listeners for orbit controls;
      window->add_listener<MouseScrollEvent>(&editor_camera.mouse_scroll_callback);
      window->add_listener<PointerDownEvent>(&editor_camera.mouse_down_callback);
      window->add_listener<PointerUpEvent>(&editor_camera.mouse_up_callback);
      window->add_listener<PointerMoveEvent>(&editor_camera.mouse_move_callback);

      window->add_listener<PointerDownEvent>(&mosue_down_callback);

      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      {
         LOG_ERROR("Failed to initialize glad.");
         throw std::runtime_error("Failed to initialize glad.");
      }
      renderer.set_viewport(0, 0, window_width, window_height);
      renderer.enable_depth_test();
      renderer.enable_blend_test();
      renderer.enable_multisample();
      renderer.enable_cull_face();
      renderer.clear_buffer();

      default_shader = new Shader(basic_material_vertex_shader, basic_material_fragment_shader);
      highlight_shader = new Shader(highlight_vertex_shader, highlight_fragment_shader);
      grid_shader = new Shader(grid_vertex_shader, grid_fragment_shader);
      grid_buffer = create_helper_grid(50.0f, 50);
   }

   void Runtime::swap_buffers()
   {
      glfwSwapBuffers(window->window_handle());
   }

   Runtime::~Runtime()
   {
      delete default_shader;
      delete highlight_shader;
      delete grid_buffer;
      delete grid_shader;
      scene.clean_up();
      delete window;
      glfwTerminate();
   }

   void Runtime::compute_mouse_click_intersections_with_world(double mouse_x_position, double mouse_y_position)
   {
      glm::vec2 window_size = window->window_size();
      glm::mat4 view_matrix = editor_camera.view();
      glm::mat4 projection_matrix = editor_camera.projection(window_size.x / window_size.y);

      glm::vec3 projected_screen_space_ray = compute_ray_from_screen_space(mouse_x_position, mouse_y_position, window_size, view_matrix, projection_matrix);

      Node *selected_node = nullptr;
      float distance_from_camera = 999999.9f;

      TransformComponent *transform_component;
      for (auto child : scene.root->child_nodes)
      {
         if (child->get_multiple_components<TransformComponent>(transform_component) &&
             ray_intersects_sphere(projected_screen_space_ray, editor_camera.position, transform_component->scale, transform_component->position))
         {
            std::cout << child->name << ": hit" << std::endl;
            auto distance = glm::length(transform_component->position - editor_camera.position);
            if (distance < distance_from_camera)
            {
               selected_node = child;
               distance_from_camera = distance;
            }
         }
      }
      if (selected_node)
      {
         scene.active_node = selected_node;
      }
      else
      {
         scene.active_node = nullptr;
      }
   }

   void Runtime::render_current_scene()
   {
      renderer.clear_buffer();
      auto window_size = window->window_size();
      renderer.set_viewport(0, 0, window_size.x, window_size.y);

      auto view_matrix = editor_camera.view();
      auto projection_matrix = editor_camera.projection(window_size.x / window_size.y);

      for (auto &node : scene.root->child_nodes)
      {
         TransformComponent *transform_component;
         MeshData *mesh_data;

         if (node->get_multiple_components<TransformComponent, MeshData>(transform_component, mesh_data))
         {
            bool selected = scene.active_node == node;

            if (selected)
            {
               renderer.enable_stencil_test();
               glStencilFunc(GL_ALWAYS, 1, 0xFF);
               glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            }
            default_shader->bind();
            default_shader->set_vec3("LightPosition", editor_camera.position);
            default_shader->set_vec3("CameraPosition", editor_camera.position);
            default_shader->set_mat4("ViewMatrix", view_matrix);
            default_shader->set_mat4("ProjectionMatrix", projection_matrix);

            glm::mat4 model_matrix(1.0f);
            glm::mat4 scale_matrix = glm::scale(model_matrix, transform_component->scale);
            glm::mat4 rotation_matrix = glm::mat4_cast(transform_component->orientation);
            glm::mat4 translate_matrix = glm::translate(model_matrix, transform_component->position);
            model_matrix = translate_matrix * (rotation_matrix * scale_matrix);

            default_shader->set_mat4("ModelMatrix", model_matrix);
            renderer.render_triangles(mesh_data->buffer);
            default_shader->unbind();

            if (selected)
            {
               glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
               glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

               highlight_shader->bind();
               highlight_shader->set_vec2("ScreenDimension", window_size);
               highlight_shader->set_mat4("ModelMatrix", model_matrix);
               highlight_shader->set_mat4("ViewMatrix", view_matrix);
               highlight_shader->set_mat4("ProjectionMatrix", projection_matrix);
               highlight_shader->set_vec3("Color", glm::vec3(1.0, 0.4, 0.0));
               renderer.render_triangles(mesh_data->buffer);
               highlight_shader->unbind();

               renderer.disable_stencil_test();
            }
         }
      }

      grid_shader->bind();
      grid_shader->set_mat4("ViewMatrix", view_matrix);
      grid_shader->set_mat4("ProjectionMatrix", projection_matrix);
      grid_shader->set_vec3("CameraPosition", editor_camera.position);
      grid_shader->set_vec3("Color", glm::vec3(0.7, 0.0, 0.0));
      renderer.render_lines(*grid_buffer, 0, 2);
      grid_shader->set_vec3("Color", glm::vec3(0.0, 0.0, 0.7));
      renderer.render_lines(*grid_buffer, 2, 2);
      grid_shader->set_vec3("Color", glm::vec3(0.4, 0.4, 0.4));
      renderer.render_lines(*grid_buffer, 4, grid_buffer->vertex_count);
      grid_shader->unbind();

      swap_buffers();
   }

   bool Runtime::is_open()
   {
      return window->is_open();
   }

   void Runtime::update()
   {
      window->poll_events();
      render_current_scene();
   }

   Runtime &Runtime::get_instance()
   {
      static Runtime instance("Synth3D", 600, 600);
      return instance;
   }
}

