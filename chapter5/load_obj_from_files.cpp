#include "glengine/application.hpp"
#include "common/object.h"
#include <math.h>

namespace chapter5
{

const char* vertex_glsl = 
"#version 410 core \n"
"layout (location = 0) in vec3 pos; \n"
"layout (location = 1) in vec3 cols; \n"
"out vec4 col; \n"
" \n"
"void main(void) \n"
"{ \n"
"  gl_Position = vec4(pos, 1.0);\n"
"  color = vec4(cols, 1.0);\n"
"} \n";

const char* fragment_glsl = 
"#version 410 core \n"
"in vec4 col; \n"
"out vec4 color; \n"
"void main(void) { \n"
"  color = col; \n"
"} \n";

class LoadObjFromFiles : public john::Application
{
public:
  LoadObjFromFiles();

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

struct Vertex {
  // Position
  float x;
  float y;
  float z;

  // color
  float r;
  float g;
  float b;

  Vertex(float _x, float _y, float _z,
      float _r, float _g, float _b) :
    x(_x),
    y(_y),
    z(_z),
    r(_r),
    g(_g),
    b(_b)
  {
  }
};

LoadObjFromFiles::LoadObjFromFiles() : 
  john::Application(),
  program_id(0)
{
}

void LoadObjFromFiles::startup()
{
  GLuint buffer;
  /*
"    vec4(0.25, -0.25, 0.5, 1.0), \n"
"    vec4(-0.25, -0.25, 0.5, 1.0), \n"
"    vec4(0.25, 0.25, 0.5, 1.0) \n"
*/

  Vertex v1(0.25, -0.25, 0.5, 1.0, 1.0, 0.0);
  Vertex v2(-0.25, -0.25, 0.5, 1.0, 1.0, 0.0);
  Vertex v3(0.25, 0.25, 0.5, 1.0, 1.0, 0.0);
  static const Vertex vertices[] = {
    v1, v2, v3
  };

  sb7::object obj;
  obj.load("media/cube.sbm");

  /*
  // create vertex array object
  glCreateVertexArrays(1, &vao);

  // init buffer obj
  glCreateBuffers(1, &buffer);
  glNamedBufferStorage(buffer, sizeof(vertices), vertices, 0);

  //set up 2 vertex attributes; first, positions:
  glVertexArrayAttribBinding(vao, 0, 0);
  glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glEnableVertexArrayAttrib(vao, 0);

  // colors
  glVertexArrayAttribBinding(vao, 1, 0);
  glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3);
  glEnableVertexArrayAttrib(vao, 1);

  // bind vao to buffer
  glVertexArrayVertexBuffer(vao, 0, buffer, 0, 0);

  program_id = compile_shaders(vertex_glsl, fragment_glsl, "", "", "");
  printf("program id: %d\n", program_id);
  */
}

void LoadObjFromFiles::render(double current_time)
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

void LoadObjFromFiles::on_key(int key, int action)
{
}

void LoadObjFromFiles::on_mouse(int button, int action) 
{
}

void LoadObjFromFiles::handle_click() 
{
}

void LoadObjFromFiles::shutdown()
{
}

}

int main(int argc, char* args[]) 
{
  chapter5::LoadObjFromFiles* app = new chapter5::LoadObjFromFiles;
  app->run(app);

  return 0;
}
