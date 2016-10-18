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
"#version 410 core                                                      \n"
"                                                                       \n"
"out VS_OUT                                                             \n"
"{                                                                      \n"
"    vec2 tc;                                                           \n"
"} vs_out;                                                              \n"
"                                                                       \n"
"uniform mat4 mvp_matrix;                                                      \n"
"uniform float offset;                                                  \n"
"                                                                       \n"
"void main(void)                                                        \n"
"{                                                                      \n"
"    const vec2[4] position = vec2[4](vec2(-0.5, -0.5),                 \n"
"                                     vec2( 0.5, -0.5),                 \n"
"                                     vec2(-0.5,  0.5),                 \n"
"                                     vec2( 0.5,  0.5));                \n"
"    vs_out.tc = (position[gl_VertexID].xy + vec2(offset, 0.5)) * vec2(30.0, 1.0);                  \n"
"    gl_Position = mvp_matrix * vec4(position[gl_VertexID], 0.0, 1.0);       \n"
"} \n";

const char* fragment_glsl = 
"#version 410 core                                                      \n"
"layout (location = 0) out vec4 color;                                  \n"
"in VS_OUT                                                              \n"
"{                                                                      \n"
"    vec2 tc;                                                           \n"
"} fs_in;                                                               \n"
"uniform sampler2D tex;                            \n"
"                                                                       \n"
"void main(void)                                                        \n"
"{                                                                      \n"
"    color = texture(tex, fs_in.tc);                                    \n"
"}                                                                      \n";

class Tunnel : public john::Application
{
public:
  Tunnel();

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
         tex_wall,
         tex_ceiling,
         tex_floor;

  float aspect;

  struct
  {
      GLint mvp_matrix;
      GLint offset;
  } uniforms;
};

Tunnel::Tunnel() : 
  john::Application(),
  program_id(0),
  vao(0),
  aspect(1268.0f / 768.0f),
  tex_wall(0),
  tex_ceiling(0),
  tex_floor(0)
{
}

void Tunnel::startup()
{
  program_id = compile_shaders(vertex_glsl, fragment_glsl);

  uniforms.mvp_matrix = glGetUniformLocation(program_id, "mvp_matrix");
  uniforms.offset = glGetUniformLocation(program_id, "offset");

  john::Application::startup();

  tex_wall = sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/src/resources/brick.ktx");
  tex_ceiling = sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/src/resources/ceiling.ktx");
  tex_floor = sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/src/resources/floor.ktx");

  int i;
  GLuint textures[] = { tex_floor, tex_wall, tex_ceiling };

  for (i = 0; i < 3; ++i) {
    glBindTexture(GL_TEXTURE_2D, textures[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    printf("texture at %d: %d\n", i, textures[i]);
  }

  glBindVertexArray(vao);

  printf("uniforms: [%d %d]\n", uniforms.mvp_matrix, uniforms.offset);
}

void Tunnel::render(double current_time) {
  const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  float t = (float) current_time;

  glViewport(0, 0, width, height);
  glClearBufferfv(GL_COLOR, 0, color);

  glm::mat4 proj = glm::perspective(60.0f, (float) width / (float) height, 0.1f, 100.0f);

  glUniform1f(uniforms.offset, t * 0.003f);
  
  int i;
  GLuint textures[] = { tex_wall, tex_floor, tex_wall, tex_ceiling};
  for (int i = 0; i < 4; ++i) {
    glm::mat4 t0 = glm::rotate(glm::mat4(), 90.0f * (float) i, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 t1 = glm::translate(t0, glm::vec3(-0.5f, 0.0f, -10.0f));
    glm::mat4 t2 = glm::rotate(t1, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 mv = glm::scale(t2, glm::vec3(30.0f, 1.0f, 1.0f));

    glm::mat4 mvp = proj * mv;
    std::cout << glm::to_string(mvp) << '\n';

    // insert `mvp` into glsl's "mvp_matrix" through `uniforms.mvp_matrix
    glUniformMatrix4fv(uniforms.mvp_matrix, 1, GL_FALSE, glm::value_ptr(mvp));

    glBindTexture(GL_TEXTURE_2D, textures[i]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }
}

void Tunnel::on_key(int key, int action)
{
}

void Tunnel::on_resize(int w, int h)
{
  width = w;
  height = h;
  aspect = (float) w / (float) h;
  //proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);
  printf("Tunnel::on_resize(%d, %d)\n", width, height);
}

void Tunnel::on_mouse(int button, int action) 
{
}

void Tunnel::handle_click() 
{
}

void Tunnel::shutdown()
{
}

}

int main(int argc, char* args[]) 
{
  chapter5::Tunnel* app = new chapter5::Tunnel;
  app->run(app);

  return 0;
}
