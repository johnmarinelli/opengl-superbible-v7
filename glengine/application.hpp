#ifndef JOHN_APPLICATION_H
#define JOHN_APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

namespace john
{

class Application
{
public:
  Application();

  virtual ~Application() {}

  virtual void startup();

  virtual void run(john::Application* current_app);

  virtual void render(double currentTime) = 0;

  virtual void shutdown();

  bool print_screen();

protected:
  static john::Application* app;

  static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    app->on_key(key, action);
  }

  void get_mouse_position(int& x, int& y)
  {
      double dx, dy;
      glfwGetCursorPos(window, &dx, &dy);

      x = static_cast<int>(floor(dx));
      y = static_cast<int>(floor(dy));
  }

  virtual void on_key(int key, int action) = 0;

  GLuint compile_shaders(const char* vtx_shdr_src, const char* frg_shdr_src, const char* tess_ctrl_src, const char* tess_eval_src);

public:
  const uint32_t width = 1024;
  const uint32_t height = 768;

  GLuint program_id;
  GLuint vao;
  GLuint palette_texture;

  GLFWwindow* window;
};

}

#endif
