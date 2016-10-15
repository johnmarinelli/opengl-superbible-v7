#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "glengine/application.hpp"
#include "common/object.h"
#include "common/sb6ktx.h"

void generate_texture(float * data, int width, int height)
{
  int x, y;

  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      data[(y * width + x) * 4 + 0] = (float)((x & y) & 0xFF) / 255.0f;
      data[(y * width + x) * 4 + 1] = (float)((x | y) & 0xFF) / 255.0f;
      data[(y * width + x) * 4 + 2] = (float)((x ^ y) & 0xFF) / 255.0f;
      data[(y * width + x) * 4 + 3] = 1.0f;
    }
  }
}

namespace chapter5
{

const char* vertex_glsl = 
"#version 410 core \n"
"const vec4 vertices[] = vec4[](vec4(-1.0, -1.0, 0.5, 1.0), \n"
"    vec4(1.0, -1.0, 0.5, 1.0), \n"
"    vec4(1.0, 1.0, 0.5, 1.0), \n"
"    vec4(-1.0,1.0, 0.5, 1.0)); \n"
" \n"
"void main(void) { \n"
"  gl_Position = vertices[gl_VertexID]; \n"
"} \n";

const char* fragment_glsl = 
"#version 410 core \n"
"uniform sampler2D s; \n"
"out vec4 color; \n"
"void main(void) { \n"
"  color = texelFetch(s, ivec2(gl_FragCoord.xy), 0); \n"
"} \n";

class TextureLoading : public john::Application
{
public:
  TextureLoading();

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
         buffer,
         texture;

  float aspect;

  glm::mat4 proj_matrix;
};

TextureLoading::TextureLoading() : 
  john::Application(),
  program_id(0),
  vao(0),
  buffer(0),
  aspect(1268.0f / 768.0f),
  proj_matrix(glm::perspective(45.0f, aspect, 0.1f, 1.0f))
{
}

void TextureLoading::startup()
{
  john::Application::startup();
  program_id = compile_shaders(vertex_glsl, fragment_glsl);

  glGenTextures(1, &texture);
  texture = sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/src/resources/pattern1.ktx", texture);
  printf("Texture: %d\n", texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  /*
  glBindTexture(GL_TEXTURE_2D, texture);

  // define storage to use for texture
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 1024, 1024);

  float* data = new float[1024 * 1024 * 4];
  generate_texture(data, 1024, 1024);

  // fill texture with data
  glTexSubImage2D(GL_TEXTURE_2D,
      0,
      0, 0,
      1024, 1024,
      GL_RGBA,
      GL_FLOAT,
      data);

  delete[] data;
  */
}

void TextureLoading::render(double current_time) {
  const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  const GLfloat one = 1.0f;

  glViewport(0, 0, width, height);
  glClearBufferfv(GL_COLOR, 0, color);

  glUseProgram(program_id);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void TextureLoading::on_key(int key, int action)
{
}

void TextureLoading::on_resize(int w, int h)
{
  width = w;
  height = h;
  aspect = (float) w / (float) h;
  //proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);
  printf("TextureLoading::on_resize(%d, %d)\n", width, height);
}

void TextureLoading::on_mouse(int button, int action) 
{
}

void TextureLoading::handle_click() 
{
}

void TextureLoading::shutdown()
{
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

}

int main(int argc, char* args[]) 
{
  chapter5::TextureLoading* app = new chapter5::TextureLoading;
  app->run(app);

  return 0;
}
