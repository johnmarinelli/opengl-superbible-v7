#include "glengine/application.hpp"
#include <math.h>

namespace chapter2
{

const char* vertex_glsl = 
"#version 450 core \n"
" \n"
"void main(void) \n"
"{ \n"
"  gl_Position = vec4(0.0, 0.0, 0.5, 1.0); \n"
"} \n";

const char* fragment_glsl = 
"#version 450 core \n"
"out vec4 color; \n"
"void main(void) { \n"
"  color = vec4(0.0, 0.8, 1.0, 1.0); \n"
"} \n";

class Colors : public john::Application
{
public:
  Colors();

  void startup();

  void render(double current_time);

  void shutdown();

  void on_mouse(int button, int action);

  void handle_click();

protected:
  void on_key(int key, int action);

private:
  GLuint program_id;
};

Colors::Colors() : 
  john::Application(),
  program_id(0)
{
}

void Colors::startup()
{
  john::Application::startup();

  program_id = compile_shaders(vertex_glsl, fragment_glsl);
}

void Colors::render(double current_time)
{
  const GLfloat color[] = { 
    (float)sin(current_time) * 0.5f + 0.5f,
    (float)cos(current_time) * 0.5f + 0.5f,
    0.0,
    1.0 
  };
  glClearBufferfv(GL_COLOR, 0, color);

  glUseProgram(program_id);
  glDrawArrays(GL_POINTS, 0, 1);
}

void Colors::on_key(int key, int action)
{
}

void Colors::on_mouse(int button, int action) 
{
}

void Colors::handle_click() 
{
}

void Colors::shutdown()
{
}

}

int main(int argc, char* args[]) 
{
  chapter2::Colors* app = new chapter2::Colors;
  app->run(app);

  return 0;
}
