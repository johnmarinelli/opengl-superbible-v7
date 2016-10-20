
#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "glengine/application.hpp"
#include "common/object.h"

namespace chapter6
{

const char* vertex_glsl = 
"#version 410 core \n"
"void main(void) \n"
"{ \n"
"    vec4 vertices[4] = vec4[4](vec4(-1.0, -1.0, 0.5, 1.0), \n"
"                               vec4( 1.0, -1.0, 0.5, 1.0), \n"
"                               vec4(-1.0,  1.0, 0.5, 1.0), \n"
"                               vec4( 1.0,  1.0, 0.5, 1.0)); \n"
" \n"
"    gl_Position = vertices[gl_VertexID]; \n"
"} \n";

const char* fragment_glsl = 
"#version 410 core                     \n"
"// First, declare the subroutine type \n"
"subroutine vec4 sub_mySubroutine(vec4 param1); \n"
" \n"
"// Next declare a coule of functions that can be used as subroutine... \n"
"subroutine (sub_mySubroutine) \n"
"vec4 myFunction1(vec4 param1) \n"
"{ \n"
"    return param1 * vec4(1.0, 0.25, 0.25, 1.0); \n"
"} \n"
" \n"
"subroutine (sub_mySubroutine) \n"
"vec4 myFunction2(vec4 param1) \n"
"{ \n"
"    return param1 * vec4(0.25, 0.25, 1.0, 1.0); \n"
"} \n"
" \n"
"// Finally, declare a subroutine uniform that can be 'pointed' \n"
"// at subroutine functions matching its signature \n"
"subroutine uniform sub_mySubroutine mySubroutineUniform; \n"
" \n"
"// Output color \n"
"out vec4 color; \n"
" \n"
"void main(void) \n"
"{ \n"
"    // Call subroutine through uniform \n"
"    color = mySubroutineUniform(vec4(1.0)); \n"
"} \n";

GLuint link_from_shaders(const GLuint * shaders,
                         int shader_count,
                         bool delete_shaders,
                         bool check_errors)
{
    int i;

    GLuint program;

    program = glCreateProgram();

    for (i = 0; i < shader_count; i++)
    {
        glAttachShader(program, shaders[i]);
    }

    glLinkProgram(program);

    if (check_errors)
    {
        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (!status)
        {
            char buffer[4096];
            glGetProgramInfoLog(program, 4096, NULL, buffer);
#ifdef _WIN32
            OutputDebugStringA(buffer);
            OutputDebugStringA("\n");
#endif
            glDeleteProgram(program);
            return 0;
        }
    }

    if (delete_shaders)
    {
        for (i = 0; i < shader_count; i++)
        {
            glDeleteShader(shaders[i]);
        }
    }

    return program;
}

class Subroutines : public john::Application
{
public:
  Subroutines();

  void startup();

  void render(double current_time);

  void shutdown();

  void on_mouse(int button, int action);

  void on_resize(int width, int height);

  void handle_click();

protected:
  void on_key(int key, int action);

private:
  GLuint render_program,
         vao;

  float aspect;

  GLuint      subroutines[2];

  struct
  {
      GLint subroutine1;
  } uniforms;
};

Subroutines::Subroutines() : 
  john::Application(),
  render_program(0),
  vao(0),
  aspect(0.0)
{
}

void Subroutines::startup()
{
  john::Application::startup();
  GLuint shaders[2];

  GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
  if (!f) printf("FShader not compiled: %d\n", f);
  glShaderSource(f, 1, &fragment_glsl, NULL);
  glCompileShader(f);

  GLint status = 0;
  glGetShaderiv(f, GL_COMPILE_STATUS, &status);

  if (!status) {
    char buffer[4096];
    glGetShaderInfoLog(f, 4096, NULL, buffer);
    fprintf(stderr, "%s\n", buffer);
    return;
  }

  GLuint v = glCreateShader(GL_VERTEX_SHADER);
  if (!v) printf("VShader not compiled: %d\n", f);
  glShaderSource(v, 1, &vertex_glsl, NULL);
  glCompileShader(v);

  status = 0;
  glGetShaderiv(v, GL_COMPILE_STATUS, &status);

  if (!status) {
    char buffer[4096];
    glGetShaderInfoLog(v, 4096, NULL, buffer);
    fprintf(stderr, "%s\n", buffer);
    return;
  }

  shaders[0] = v;
  shaders[1] = f;

  if (render_program)
    glDeleteProgram(render_program);

  render_program = link_from_shaders(shaders, 2, true, true);

  subroutines[0] = glGetSubroutineIndex(render_program, GL_FRAGMENT_SHADER, "myFunction1");
  subroutines[1] = glGetSubroutineIndex(render_program, GL_FRAGMENT_SHADER, "myFunction2");

  uniforms.subroutine1 = glGetSubroutineUniformLocation(render_program, GL_FRAGMENT_SHADER, "mySubroutineUniform");
}

void Subroutines::render(double current_time) {
  const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  const GLfloat one = 1.0f;

  glViewport(0, 0, width, height);
  glClearBufferfv(GL_COLOR, 0, color);

  int i = int(current_time);
  glUseProgram(render_program);

  glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &subroutines[i & 1]);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Subroutines::on_key(int key, int action)
{
  if (action) {
    switch (key) {
      case 'R': startup(); break;
    }
  }
}

void Subroutines::on_resize(int w, int h)
{
}

void Subroutines::on_mouse(int button, int action) 
{
}

void Subroutines::handle_click() 
{
}

void Subroutines::shutdown()
{
}

}

int main(int argc, char* args[]) 
{
  chapter6::Subroutines* app = new chapter6::Subroutines;
  app->run(app);

  return 0;
}
