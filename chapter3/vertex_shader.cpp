#include "glengine/application.hpp"
#include <math.h>

namespace chapter3
{

const char* vertex_glsl = 
"#version 410 core \n"
" \n"
"// 'offset', 'color' is an input vertex attribute; \n"
"// set 'offset' and 'color' by user glVertexAttrib4fv({0,1}, val) \n"
"layout (location = 0) in vec4 offset; \n"
"layout (location = 1) in vec4 color; \n"
"// output interface block \n"
"// sends color to frag shader \n"
"out VS_OUT \n"
"{ \n"
"vec4 vs_color; \n"
"} vs_out; \n"
"void main(void) \n"
"{ \n"
"  const vec4 vertices[] = vec4[3]( \n"
"    vec4(0.25, -0.25, 0.5, 1.0), \n"
"    vec4(-0.25, -0.25, 0.5, 1.0), \n"
"    vec4(0.25, 0.25, 0.5, 1.0) \n"
"  ); \n"
"  gl_Position = vertices[gl_VertexID] + offset;\n"
"  vs_out.vs_color = color; \n"
"} \n";

const char* fragment_glsl = 
"#version 410 core \n"
"// receive color from vertex shader \n"
"in VS_OUT \n "
"{ \n"
"vec4 vs_color; \n"
"} fs_in; \n"
"out vec4 color; \n"
"void main(void) { \n"
"  color = fs_in.vs_color; \n"
"} \n";


class VertexShader : public john::Application
{
public:
  VertexShader();

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

VertexShader::VertexShader() : 
  john::Application(),
  program_id(0)
{
}

void VertexShader::startup()
{
  john::Application::startup();

  program_id = compile_shaders(vertex_glsl, fragment_glsl);
  printf("program id: %d\n", program_id);
}

void VertexShader::render(double current_time)
{
  const GLfloat bg_color[] = { 
//    (float)sin(current_time) * 0.5f + 0.5f,
//    (float)cos(current_time) * 0.5f + 0.5f,
    0.0,
    0.0,
    0.0,
    1.0 
  };

  glClearBufferfv(GL_COLOR, 0, bg_color);

  glUseProgram(program_id);
  GLfloat attrib[] = { (float) sin(current_time) * 0.5f,
    (float)cos(current_time) * 0.6f,
    0.0f, 0.0f };

  // update value of input attribute 0
  glVertexAttrib4fv(0, attrib);

  GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };
  glVertexAttrib4fv(1, color);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void VertexShader::on_key(int key, int action)
{
}

void VertexShader::on_mouse(int button, int action) 
{
}

void VertexShader::handle_click() 
{
}

void VertexShader::shutdown()
{
}

}

int main(int argc, char* args[]) 
{
  chapter3::VertexShader* app = new chapter2::VertexShader;
  app->run(app);

  return 0;
}
