#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "glengine/application.hpp"
#include "common/object.h"
#include "common/sb6ktx.h"

namespace chapter7
{

const char* vertex_glsl = 
"// Vertex Shader                                                                                            \n"
"// Graham Sellers                                                                                           \n"
"// OpenGL SuperBible                                                                                        \n"
"#version 410 core                                                                                           \n"
"                                                                                                            \n"
"// Incoming per vertex position                                                                             \n"
"in vec4 vVertex;                                                                                            \n"
"                                                                                                            \n"
"// Output varyings                                                                                          \n"
"out vec4 color;                                                                                             \n"
"                                                                                                            \n"
"uniform mat4 mvpMatrix;                                                                                     \n"
"                                                                                                            \n"
"uniform sampler1D grasspalette_texture;                                                \n"
"uniform sampler2D length_texture;                                                      \n"
"uniform sampler2D orientation_texture;                                                 \n"
"uniform sampler2D grasscolor_texture;                                                  \n"
"uniform sampler2D bend_texture;                                                        \n"
"                                                                                                            \n"
"int random(int seed, int iterations)                                                                        \n"
"{                                                                                                           \n"
"    int value = seed;                                                                                       \n"
"    int n;                                                                                                  \n"
"                                                                                                            \n"
"    for (n = 0; n < iterations; n++) {                                                                      \n"
"        value = ((value >> 7) ^ (value << 9)) * 15485863;                                                   \n"
"    }                                                                                                       \n"
"                                                                                                            \n"
"    return value;                                                                                           \n"
"}                                                                                                           \n"
"                                                                                                            \n"
"vec4 random_vector(int seed)                                                                                \n"
"{                                                                                                           \n"
"    int r = random(gl_InstanceID, 4);                                                                       \n"
"    int g = random(r, 2);                                                                                   \n"
"    int b = random(g, 2);                                                                                   \n"
"    int a = random(b, 2);                                                                                   \n"
"                                                                                                            \n"
"    return vec4(float(r & 0x3FF) / 1024.0,                                                                  \n"
"                float(g & 0x3FF) / 1024.0,                                                                  \n"
"                float(b & 0x3FF) / 1024.0,                                                                  \n"
"                float(a & 0x3FF) / 1024.0);                                                                 \n"
"}                                                                                                           \n"
"                                                                                                            \n"
"mat4 construct_rotation_matrix(float angle)                                                                 \n"
"{                                                                                                           \n"
"    float st = sin(angle);                                                                                  \n"
"    float ct = cos(angle);                                                                                  \n"
"                                                                                                            \n"
"    return mat4(vec4(ct, 0.0, st, 0.0),                                                                     \n"
"                vec4(0.0, 1.0, 0.0, 0.0),                                                                   \n"
"                vec4(-st, 0.0, ct, 0.0),                                                                    \n"
"                vec4(0.0, 0.0, 0.0, 1.0));                                                                  \n"
"}                                                                                                           \n"
"                                                                                                            \n"
"void main(void)                                                                                             \n"
"{                                                                                                           \n"
"    vec4 offset = vec4(float(gl_InstanceID >> 10) - 512.0,                                                  \n"
"                       0.0f,                                                                                \n"
"                       float(gl_InstanceID & 0x3FF) - 512.0,                                                \n"
"                       0.0f);                                                                               \n"
"    int number1 = random(gl_InstanceID, 3);                                                                 \n"
"    int number2 = random(number1, 2);                                                                       \n"
"    offset += vec4(float(number1 & 0xFF) / 256.0,                                                           \n"
"                   0.0f,                                                                                    \n"
"                   float(number2 & 0xFF) / 256.0,                                                           \n"
"                   0.0f);                                                                                   \n"
"    // float angle = float(random(number2, 2) & 0x3FF) / 1024.0;                                            \n"
"                                                                                                            \n"
"    vec2 texcoord = offset.xz / 1024.0 + vec2(0.5);                                                         \n"
"                                                                                                            \n"
"    // float bend_factor = float(random(number2, 7) & 0x3FF) / 1024.0;                                      \n"
"    float bend_factor = texture(bend_texture, texcoord).r * 2.0;                                            \n"
"    float bend_amount = cos(vVertex.y);                                                                     \n"
"                                                                                                            \n"
"    float angle = texture(orientation_texture, texcoord).r * 2.0 * 3.141592;                                \n"
"    mat4 rot = construct_rotation_matrix(angle);                                                            \n"
"    vec4 position = (rot * (vVertex + vec4(0.0, 0.0, bend_amount * bend_factor, 0.0))) + offset;            \n"
"                                                                                                            \n"
"    position *= vec4(1.0, texture(length_texture, texcoord).r * 0.9 + 0.3, 1.0, 1.0);                       \n"
"                                                                                                            \n"
"    gl_Position = mvpMatrix * position; // (rot * position);                                                \n"
"    // color = vec4(random_vector(gl_InstanceID).xyz * vec3(0.1, 0.5, 0.1) + vec3(0.1, 0.4, 0.1), 1.0);     \n"
"    // color = texture(orientation_texture, texcoord);                                                      \n"
"    color = texture(grasspalette_texture, texture(grasscolor_texture, texcoord).r) +                        \n"
"            vec4(random_vector(gl_InstanceID).xyz * vec3(0.1, 0.5, 0.1), 1.0);                              \n"
"}                                                                                                           \n";

const char* fragment_glsl = 
"// Fragment Shader               \n"
"// Graham Sellers                \n"
"// OpenGL SuperBible             \n"
"#version 410 core                \n"
"                                 \n"
"in vec4 color;                   \n"
"                                 \n"
"out vec4 output_color;           \n"
"                                 \n"
"void main(void)                  \n"
"{                                \n"
"    output_color = color;        \n"
"}                                \n";

class Grass : public john::Application
{
public:
  Grass();

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
         grass_buffer,
         tex_grass_color,
         tex_grass_length,
         tex_grass_orientation,
         tex_grass_bend;

  struct
  {
    GLint mvpMatrix;
  } uniforms;

  GLint mv_location,
        proj_location;

  float aspect;

  glm::mat4 proj_matrix;
};

Grass::Grass() : 
  john::Application(),
  program_id(0),
  vao(0),
  grass_buffer(0),
  tex_grass_length(0),
  tex_grass_color(0),
  tex_grass_bend(0),
  tex_grass_orientation(0),
  aspect(1268.0f / 768.0f),
  proj_matrix(glm::perspective(50.0f, aspect, 0.1f, 1000.0f))
{
}

void Grass::startup()
{

  static const GLfloat grass_blade[] =
  {
    -0.3f, 0.0f,
     0.3f, 0.0f,
    -0.20f, 1.0f,
     0.1f, 1.3f,
    -0.05f, 2.3f,
     0.0f, 3.3f
  };

  glGenBuffers(1, &grass_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, grass_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(grass_blade), grass_blade, GL_STATIC_DRAW);

  john::Application::startup();

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  program_id = compile_shaders(vertex_glsl, fragment_glsl);

  uniforms.mvpMatrix = glGetUniformLocation(program_id, "mvpMatrix");

  GLint location1 = glGetUniformLocation(program_id, "grasspalette_texture");
  GLint location2 = glGetUniformLocation(program_id, "length_texture");
  GLint location3 = glGetUniformLocation(program_id, "orientation_texture");
  GLint location4 = glGetUniformLocation(program_id, "grasscolor_texture");
  GLint location5 = glGetUniformLocation(program_id, "bend_texture");
  printf("%d %d %d %d %d \n", location1, location2, location3, location4, location5);

  glUniform1i(location1, 0);
  glUniform1i(location2, 1);
  glUniform1i(location3, 2);
  glUniform1i(location4, 3);
  glUniform1i(location5, 4);

  glActiveTexture(GL_TEXTURE0 + 1);
  tex_grass_length = sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/media/textures/grass_length.ktx");
  glBindTexture(GL_TEXTURE_1D, tex_grass_length);

  glActiveTexture(GL_TEXTURE0 + 2);
  tex_grass_orientation = sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/media/textures/grass_orientation.ktx");
  glBindTexture(GL_TEXTURE_2D, tex_grass_orientation);

  glActiveTexture(GL_TEXTURE0 + 3);
  tex_grass_color = sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/media/textures/grass_color.ktx");
  glBindTexture(GL_TEXTURE_2D, tex_grass_color);

  glActiveTexture(GL_TEXTURE0 + 4);
  tex_grass_bend = sb6::ktx::file::load("/Local/Users/john/Documents/cpp/superbible/media/textures/grass_bend.ktx");
  glBindTexture(GL_TEXTURE_2D, tex_grass_bend);


  printf("%d %d %d \n", tex_grass_orientation, tex_grass_color, tex_grass_bend);


  glEnable(GL_CULL_FACE);
  //glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

}

void Grass::render(double current_time) {
  float t = (float) current_time * 0.02f;
  float r = 550.0f;

  static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
  static const GLfloat one = 1.0f;
  glClearBufferfv(GL_COLOR, 0, black);
  glClearBufferfv(GL_DEPTH, 0, &one);

  glm::mat4 mv_matrix = glm::lookAt(glm::vec3(sinf(t) * r, 25.0f, cosf(t) * r),
      glm::vec3(0.0f, -50.0f, 0.0f),
      glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 prj_matrix = glm::perspective(45.0f, (float) width / (float) height, 0.1f, 1000.0f);

  glUseProgram(program_id);
  glUniformMatrix4fv(uniforms.mvpMatrix, 1, GL_FALSE, glm::value_ptr(prj_matrix * mv_matrix));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glViewport(0, 0, width, height);
  glBindVertexArray(vao);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, 1024 * 1024);
}

void Grass::on_key(int key, int action)
{
}

void Grass::on_resize(int w, int h)
{
  width = w;
  height = h;
  aspect = (float) w / (float) h;
  proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);
  printf("Grass::on_resize(%d, %d)\n", width, height);
}

void Grass::on_mouse(int button, int action) 
{
}

void Grass::handle_click() 
{
}

void Grass::shutdown()
{
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

}

int main(int argc, char* args[]) 
{
  chapter7::Grass* app = new chapter7::Grass;
  app->run(app);

  return 0;
}
