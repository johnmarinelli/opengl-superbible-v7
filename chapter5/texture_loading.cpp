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
"in vec4 position; \n"
"in vec2 tc; \n"
" \n"
" \n"
"out VS_OUT \n"
"{ \n"
"    vec2 tc; \n"
"} vs_out; \n"
"uniform mat4 mv_matrix; \n"
"uniform mat4 proj_matrix; \n"
" \n"
" \n"
"void main(void) \n"
"{ \n"
"    vec4 pos_vs = mv_matrix * position; \n"
" \n"
"    vs_out.tc = tc; \n"
" \n"
"    gl_Position = proj_matrix * pos_vs; \n"
"} \n";
/*
"#version 410 core \n"
"const vec4 vertices[] = vec4[](vec4(-1.0, -1.0, 0.5, 1.0), \n"
"    vec4(1.0, -1.0, 0.5, 1.0), \n"
"    vec4(1.0, 1.0, 0.5, 1.0), \n"
"    vec4(-1.0,1.0, 0.5, 1.0)); \n"
" \n"
"void main(void) { \n"
"  gl_Position = vertices[gl_VertexID]; \n"
"} \n";
*/

const char* fragment_glsl = 
"#version 410 core \n"
" \n"
"uniform sampler2D tex_object; \n"
" \n"
"in VS_OUT \n"
"{ \n"
"    vec2 tc; \n"
"} fs_in; \n"
" \n"
"out vec4 color; \n"
" \n"
"void main(void) \n"
"{ \n"
"    color = texture(tex_object, fs_in.tc * vec2(3.0, 1.0)); \n"
"} \n";
/*
"#version 410 core \n"
"uniform sampler2D s; \n"
"out vec4 color; \n"
"void main(void) { \n"
"  color = texelFetch(s, ivec2(gl_FragCoord.xy), 0); \n"
"} \n";
*/

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
  sb6::object object;
  GLuint tex_object[2];

  struct
  {
      GLint       mv_matrix;
      GLint       proj_matrix;
  } uniforms;

  glm::mat4 proj_matrix;
};

TextureLoading::TextureLoading() : 
  john::Application(),
  program_id(0),
  vao(0),
  buffer(0),
  aspect(1268.0f / 768.0f),
  proj_matrix(glm::perspective(45.0f, 1268.0f / 768.0f, 0.1f, 10.0f))
{
}

void TextureLoading::startup()
{
#define B 0x00, 0x00, 0x00, 0x00
#define W 0xFF, 0xFF, 0xFF, 0xFF
  static const GLubyte tex_data[] =
  {
    B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
    B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
    B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
    B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
    B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
    B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
    B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
    B, W, B, W, B, W, B, W, B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B, W, B, W, B, W, B, W, B,
  };
#undef B
#undef W

  john::Application::startup();
  program_id = compile_shaders(vertex_glsl, fragment_glsl);

  glGenTextures(1, &tex_object[0]);
  glBindTexture(GL_TEXTURE_2D, tex_object[0]);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 16, 16);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 16, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  tex_object[1] = sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/src/resources/pattern1.ktx", texture);

  object.load("/Local/Users/john/Documents/cpp/superbible/media/torus_nrms_tc.sbm");

  printf("program_id: %d\n", program_id);

  uniforms.mv_matrix = glGetUniformLocation(program_id, "mv_matrix");
  uniforms.proj_matrix = glGetUniformLocation(program_id, "proj_matrix");

  printf("uniforms: [%d %d]\n", uniforms.mv_matrix, uniforms.proj_matrix);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  //texture = sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/src/resources/pattern1.ktx", texture);

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
  const GLfloat ones[] = { 1.0f };

  glClearBufferfv(GL_COLOR, 0, color);
  glClearBufferfv(GL_DEPTH, 0, ones);

  glViewport(0, 0, width, height);

  //glBindTexture(GL_TEXTURE_2D, tex_object[1]);
  glBindTexture(GL_TEXTURE_2D, tex_object[0]);

  glUseProgram(program_id);

  glm::mat4 t0 = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 t1 = glm::rotate(t0, (float) current_time * 1.3f, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 t2 = glm::rotate(t1, (float) current_time * 2.1f, glm::vec3(0.0f, 0.0f, 1.0f));

  glUniformMatrix4fv(uniforms.mv_matrix, 1, GL_FALSE, glm::value_ptr(t2));
  glUniformMatrix4fv(uniforms.proj_matrix, 1, GL_FALSE, glm::value_ptr(proj_matrix));

  object.render();
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
