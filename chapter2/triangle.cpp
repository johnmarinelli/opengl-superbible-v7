#include "glengine/application.hpp"
#include <math.h>

namespace chapter2
{

const char* vertex_glsl = 
"#version 410 core \n"
" \n"
"void main(void) \n"
"{ \n"
"  const vec4 vertices[] = vec4[3]( \n"
"    vec4(0.25, -0.25, 0.5, 1.0), \n"
"    vec4(-0.25, -0.25, 0.5, 1.0), \n"
"    vec4(0.25, 0.25, 0.5, 1.0) \n"
"  ); \n"
"  gl_Position = vertices[gl_VertexID];\n"
"} \n";

const char* fragment_glsl = 
"#version 410 core \n"
"out vec4 color; \n"
"void main(void) { \n"
"  color = vec4(0.0, 0.2, 1.0, 1.0); \n"
"} \n";

class Triangle : public john::Application
{
public:
  Triangle();

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

Triangle::Triangle() : 
  john::Application(),
  program_id(0)
{
}

void Triangle::startup()
{
  john::Application::startup();

  program_id = compile_shaders(vertex_glsl, fragment_glsl);
  printf("program id: %d\n", program_id);
}

void Triangle::render(double current_time)
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
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::on_key(int key, int action)
{
}

void Triangle::on_mouse(int button, int action) 
{
}

void Triangle::handle_click() 
{
}

void Triangle::shutdown()
{
}

}

int main(int argc, char* args[]) 
{
  chapter2::Triangle* app = new chapter2::Triangle;
  app->run(app);

  return 0;
}
