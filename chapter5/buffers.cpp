#include "glengine/application.hpp"
#include "common/object.h"
#include <math.h>

namespace chapter5
{

const char* vertex_glsl = 
"#version 330 core \n"
"layout (location = 0) in vec4 position; \n"
"void main() { \n"
"  gl_Position = position; \n"
"} \n";
/*"layout (location = 0) in vec3 pos; \n"
"layout (location = 1) in vec3 cols; \n"
"out vec4 col; \n"
" \n"
"void main(void) \n"
"{ \n"
"  gl_Position = vec4(pos, 1.0);\n"
"  color = vec4(cols, 1.0);\n"
"} \n";*/

const char* fragment_glsl = 
"#version 330 core \n"
"out vec4 color; \n"
"void main(void) { \n"
"  color = vec4(1.0, 0.0, 1.0, 1.0); \n"
"} \n";

class Buffers : public john::Application
{
public:
  Buffers();

  void startup();

  void render(double current_time);

  void shutdown();

  void on_mouse(int button, int action);

  void handle_click();

protected:
  void on_key(int key, int action);

private:
  GLuint buffer;
  GLuint program_id;
};

Buffers::Buffers() : 
  john::Application(),
  program_id(0),
  buffer(0)
{
}

void Buffers::startup()
{
  john::Application::startup();

  static const float data[] = {
    1.0f, 1.0f, 0.0f,
    1.0f, -0.75f, 0.0f,
    -1.0f, -1.0f, 0.0f
  };

  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
  program_id = compile_shaders(vertex_glsl, fragment_glsl);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Buffers::render(double current_time)
{
  glBindVertexArray(vao);
  const GLfloat color[] = { 
//    (float)sin(current_time) * 0.5f + 0.5f,
//    (float)cos(current_time) * 0.5f + 0.5f,
    0.0,
    0.0,
    1.0,
    0.0 
  };

  glClearBufferfv(GL_COLOR, 0, color);
  glUseProgram(program_id);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Buffers::on_key(int key, int action)
{
}

void Buffers::on_mouse(int button, int action) 
{
}

void Buffers::handle_click() 
{
}

void Buffers::shutdown()
{
}

}

int main(int argc, char* args[]) 
{
  chapter5::Buffers* app = new chapter5::Buffers;
  app->run(app);

  return 0;
}
