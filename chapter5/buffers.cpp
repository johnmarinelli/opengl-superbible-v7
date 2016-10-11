#include "glengine/application.hpp"
#include "common/object.h"
#include <math.h>

namespace chapter5
{

const char* vertex_glsl = 
"#version 330 core \n"
"layout (location = 0) in vec4 position; \n"
"layout (location = 1) in vec3 color; \n"
"out vec3 out_color; \n"
"void main() { \n"
"  gl_Position = position; \n"
"  out_color = color; \n"
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
"in vec3 out_color; \n"
"out vec3 color; \n"
"void main(void) { \n"
"  color = out_color; \n"
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
  GLuint buffer_2;
  GLuint buffer_3;
  GLuint program_id;
};

Buffers::Buffers() : 
  john::Application(),
  program_id(0),
  buffer(0)
{
}

struct Vertex {
  float x, y, z, 
    r, g, b;
};

void Buffers::startup()
{
  john::Application::startup();

  static const float positions[] = {
    1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f
  };

  static const float colors[] = {
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f
  };

  static const Vertex vertices[] = {
    { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
    { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f }
  };
  /*
  glGenBuffers(1, &buffer);

  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
  */

  /*
  glGenBuffers(1, &buffer);

  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &buffer_2);

  glBindBuffer(GL_ARRAY_BUFFER, buffer_2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(1);
  */
  program_id = compile_shaders(vertex_glsl, fragment_glsl);
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
  glEnableVertexAttribArray(1);

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
  /*
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  */

  /*
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_2);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
  */

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
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

}

int main(int argc, char* args[]) 
{
  chapter5::Buffers* app = new chapter5::Buffers;
  app->run(app);

  return 0;
}
