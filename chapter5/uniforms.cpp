#include "glengine/application.hpp"
#include "common/object.h"
#include <math.h>

namespace chapter5
{

const char* vertex_glsl = 
"#version 410 core \n"
"layout (location = 0) in vec4 position; \n"
"layout (location = 1) in vec3 color; \n"
"uniform float blue; \n"
"uniform vec3 uniform_color; \n"
"/*layout(std140, align = 16) ManuallyLaidOutBlock \n"
"{ \n"
"  layout (offset = 32) vec4 foo; // 32 % 16 == 0 \n"
"  layout (offset = 8) vec2 bar; // 8 % 8 == 0 \n"
"  layout (offset = 48) vec3 baz; // 48 % 16 == 0 \n"
"} manually_laid_out;*/ \n"
/*"layout(std140) uniform TransformBlock \n"
"{ \n"
"  float scale; \n"
"  vec3 translation; \n"
"  float rotation[3]; \n"
"  mat4 projection_matrix; \n"
"} transform; \n"*/
"layout(std140, binding = 2) uniform TransformBlock \n"
"{ \n"
"  float scale; \n"
"  vec3 translation; \n"
"  float rotation[3]; \n"
"  mat4 projection_matrix; \n"
"} transform; \n"
"out vec3 out_color; \n"
"void main() { \n"
"  gl_Position = position; \n"
"  out_color = uniform_color; \n"
"} \n";

const char* fragment_glsl = 
"#version 410 core \n"
"in vec3 out_color; \n"
"out vec3 color; \n"
"void main(void) { \n"
"  color = out_color; \n"
"} \n";

class Uniforms : public john::Application
{
public:
  Uniforms();

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
  GLuint blue;
  GLuint uniform_color;
};

Uniforms::Uniforms() : 
  john::Application(),
  program_id(0),
  buffer(0)
{
}

struct Vertex {
  float x, y, z, 
    r, g, b;
};

void Uniforms::startup()
{
  john::Application::startup();

  static const Vertex vertices[] = {
    { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
    { 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f },
    { -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f }
  };

  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
  glEnableVertexAttribArray(0);
  program_id = compile_shaders(vertex_glsl, fragment_glsl);

  static const char* uniform_names[4] = {
    "TransformBlock.scale",
    "TransformBlock.translation",
    "TransformBlock.rotation",
    "TransformBlock.projection_matrix"
  };

  GLuint uniform_indices[4];
  glGetUniformIndices(program_id, 1, uniform_names, uniform_indices);

  // @wip
}

  glClearBufferfv(GL_COLOR, 0, color);
  glUseProgram(program_id);

  blue = glGetUniformLocation(program_id, "blue");
  GLfloat bval = 0.5;
  glUniform1fv(blue, 1, &bval);

  uniform_color = glGetUniformLocation(program_id, "uniform_color");
  GLfloat uc[] = { 1.0, 0.5, 0.0 };
  glUniform3fv(uniform_color, 1, uc);

  glDrawArrays(GL_TRIANGLES, 0, 3);

}

void Uniforms::on_key(int key, int action)
{
}

void Uniforms::on_mouse(int button, int action) 
{
}

void Uniforms::handle_click() 
{
}

void Uniforms::shutdown()
{
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

}

int main(int argc, char* args[]) 
{
  chapter5::Uniforms* app = new chapter5::Uniforms;
  app->run(app);

  return 0;
}
