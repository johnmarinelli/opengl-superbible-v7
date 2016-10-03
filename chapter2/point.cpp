#include "glengine/application.hpp"
#include <math.h>

namespace chapter2
{

const char* vertex_glsl = 
"#version 410 core \n"
" \n"
"void main(void) \n"
"{ \n"
"  gl_Position = vec4(0.0, 0.0, 0.0, 1.0); \n"
"} \n";

const char* fragment_glsl = 
"#version 410 core \n"
"out vec4 color; \n"
"void main(void) { \n"
"  color = vec4(0.0, 0.2, 1.0, 1.0); \n"
"} \n";

class Point : public john::Application
{
public:
  Point();

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

Point::Point() : 
  john::Application(),
  program_id(0)
{
}

void Point::startup()
{
  john::Application::startup();

  program_id = compile_shaders(vertex_glsl, fragment_glsl);
  printf("program id: %d\n", program_id);
}

void Point::render(double current_time)
{
  const GLfloat color[] = { 
//    (float)sin(current_time) * 0.5f + 0.5f,
//    (float)cos(current_time) * 0.5f + 0.5f,
    0.0,
    0.0,
    0.0,
    1.0 
  };
  glClearBufferfv(GL_COLOR, 0, color);

  glUseProgram(program_id);
  glPointSize(40.0f);
  glDrawArrays(GL_POINTS, 0, 1);
}

void Point::on_key(int key, int action)
{
}

void Point::on_mouse(int button, int action) 
{
}

void Point::handle_click() 
{
}

void Point::shutdown()
{
}

}

int main(int argc, char* args[]) 
{
  chapter2::Point* app = new chapter2::Point;
  app->run(app);

  return 0;
}
