#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "glengine/application.hpp"
#include "common/object.h"
#include "common/sb6ktx.h"

namespace chapter5
{

const char* vertex_glsl = 
"#version 410 core                                                \n"
"                                                                 \n"
"uniform vec2 offset;                                             \n"
"                                                                 \n"
"out vec2 tex_coord;                                              \n"
"                                                                 \n"
"void main(void)                                                  \n"
"{                                                                \n"
"    const vec4 vertices[] = vec4[](vec4(-0.45, -0.45, 0.5, 1.0), \n"
"                                   vec4( 0.45, -0.45, 0.5, 1.0), \n"
"                                   vec4(-0.45,  0.45, 0.5, 1.0), \n"
"                                   vec4( 0.45,  0.45, 0.5, 1.0));\n"
"                                                                 \n"
"    gl_Position = vertices[gl_VertexID] + vec4(offset, 0.0, 0.0);\n"
"    tex_coord = vertices[gl_VertexID].xy * 3.0 + vec2(0.45 * 3); \n"
"}                                                                \n";

const char* fragment_glsl = 
"#version 410 core                     \n"
"                                      \n"
"uniform sampler2D s;                  \n"
"                                      \n"
"out vec4 color;                       \n"
"                                      \n"
"in vec2 tex_coord;                    \n"
"                                      \n"
"void main(void)                       \n"
"{                                     \n"
"    color = texture(s, tex_coord);    \n"
"}                                     \n";

class WrapModes : public john::Application
{
public:
  WrapModes();

  void startup();

  void render(double current_time);

  void shutdown();

  void on_mouse(int button, int action);

  void on_resize(int width, int height);

  void handle_click();

protected:
  void on_key(int key, int action);

private:
  GLuint program_id,
         vao,
         texture;

  float aspect;
};

WrapModes::WrapModes() : 
  john::Application(),
  program_id(0),
  vao(0),
  texture(0),
  aspect(1268.0f / 768.0f)
{
}

void WrapModes::startup()
{
  john::Application::startup();
  program_id = compile_shaders(vertex_glsl, fragment_glsl);

  // Generate a name for the texture
  glGenTextures(1, &texture);

  // Load texture from file
  sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/src/resources/rightarrows.ktx", texture);

  glBindTexture(GL_TEXTURE_2D, texture);
}

void WrapModes::render(double current_time) {
  static const GLfloat green[] = { 0.0f, 0.1f, 0.0f, 1.0f };
  static const GLfloat yellow[] = { 0.4f, 0.4f, 0.0f, 1.0f };
  glClearBufferfv(GL_COLOR, 0, green);

  static const GLenum wrapmodes[] = { GL_CLAMP_TO_EDGE, GL_REPEAT, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT };
  static const float offsets[] = { -0.5f, -0.5f,
                                    0.5f, -0.5f,
                                   -0.5f,  0.5f,
                                    0.5f,  0.5f };

  glUseProgram(program_id);
  glViewport(0, 0, width, height);

  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, yellow);

  for (int i = 0; i < 4; i++)
  {
      glUniform2fv(0, 1, &offsets[i * 2]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmodes[i]);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmodes[i]);

      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }

}

void WrapModes::on_key(int key, int action)
{
}

void WrapModes::on_resize(int w, int h)
{
  width = w;
  height = h;
  aspect = (float) w / (float) h;
  //proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);
  printf("WrapModes::on_resize(%d, %d)\n", width, height);
}

void WrapModes::on_mouse(int button, int action) 
{
}

void WrapModes::handle_click() 
{
}

void WrapModes::shutdown()
{
}

}

int main(int argc, char* args[]) 
{
  chapter5::WrapModes* app = new chapter5::WrapModes;
  app->run(app);

  return 0;
}
