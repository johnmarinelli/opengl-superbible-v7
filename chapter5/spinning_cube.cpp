#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>

#include "glengine/application.hpp"
#include "common/object.h"

namespace chapter5
{

const char* vertex_glsl = 
"#version 410 core \n"
" \n"
"in vec4 position; \n"
" \n"
"out VS_OUT \n"
"{ \n"
"  vec4 color; \n"
"} vs_out; \n"
" \n"
"uniform mat4 mv_matrix; \n"
"uniform mat4 proj_matrix; \n"
" \n"
"void main(void) { \n"
"  gl_Position = proj_matrix * mv_matrix * position; \n"
"  vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);      \n"
"} \n" ;

const char* fragment_glsl = 
"#version 410 core \n"
" \n"
"out vec4 color; \n"
" \n"
"in VS_OUT \n"
"{ \n"
"  vec4 color; \n"
"} fs_in; \n"
" \n"
"void main(void) { \n"
"  color = fs_in.color; \n"
"} \n";

class SpinningCube : public john::Application
{
public:
  SpinningCube();

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
         buffer;

  GLint mv_location,
        proj_location;

  float aspect;

  glm::mat4 proj_matrix;
};

SpinningCube::SpinningCube() : 
  john::Application(),
  program_id(0),
  vao(0),
  buffer(0),
  mv_location(0),
  proj_location(0),
  aspect(1268.0f / 768.0f),
  proj_matrix(glm::perspective(50.0f, aspect, 0.1f, 1000.0f))
{
}

void SpinningCube::startup()
{
  program_id = compile_shaders(vertex_glsl, fragment_glsl);

  mv_location = glGetUniformLocation(program_id, "mv_matrix");
  proj_location = glGetUniformLocation(program_id, "proj_matrix");

  printf("mv_location: %d proj_location %d\n", mv_location, proj_location);

  john::Application::startup();

  static const GLfloat vertex_positions[] = {
    -0.25f,  0.25f, -0.25f,
    -0.25f, -0.25f, -0.25f,
     0.25f, -0.25f, -0.25f,

     0.25f, -0.25f, -0.25f,
     0.25f,  0.25f, -0.25f,
    -0.25f,  0.25f, -0.25f,

     0.25f, -0.25f, -0.25f,
     0.25f, -0.25f,  0.25f,
     0.25f,  0.25f, -0.25f,

     0.25f, -0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,
     0.25f,  0.25f, -0.25f,

     0.25f, -0.25f,  0.25f,
    -0.25f, -0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f,  0.25f,
    -0.25f,  0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f,  0.25f,
    -0.25f, -0.25f, -0.25f,
    -0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f, -0.25f,
    -0.25f,  0.25f, -0.25f,
    -0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f,  0.25f,
     0.25f, -0.25f,  0.25f,
     0.25f, -0.25f, -0.25f,

     0.25f, -0.25f, -0.25f,
    -0.25f, -0.25f, -0.25f,
    -0.25f, -0.25f,  0.25f,

    -0.25f,  0.25f, -0.25f,
     0.25f,  0.25f, -0.25f,
     0.25f,  0.25f,  0.25f,

     0.25f,  0.25f,  0.25f,
    -0.25f,  0.25f,  0.25f,
    -0.25f,  0.25f, -0.25f
  };

  // generate data, and insert it into buffer
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER,
      sizeof(vertex_positions),
      vertex_positions,
      GL_STATIC_DRAW);

  // set up vertex attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

}

void SpinningCube::render(double current_time) {
  const GLfloat color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  const GLfloat one = 1.0f;

  glViewport(0, 0, width, height);
  glClearBufferfv(GL_DEPTH, 0, &one);
  glClearBufferfv(GL_COLOR, 0, color);

  glUseProgram(program_id);
  glm::mat4 identity_matrix = glm::mat4(1.0f);
  glm::mat4 view_matrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -100.0f));
  glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj_matrix));
#define LOTS_O_CUBES

#ifdef LOTS_O_CUBES
  int i;
  for (i = 0; i < 24; ++i) {
    float f = (float)i + (float)current_time * 0.3f;

    glm::mat4 view_translation_matrix = glm::translate(
      view_matrix,
      glm::vec3(
        sinf(2.1f * f) * 2.0f,
        cosf(1.7f * f) * 2.0f,                        
        sinf(1.3f * f) * cosf(1.5f * f) * 2.0f    
      )
    );

    glm::mat4 rotated_vt_matrix = glm::rotate(
      view_translation_matrix,
      (float) current_time * 45.0f,
      glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glm::mat4 mv_matrix = glm::rotate(
      rotated_vt_matrix,
      (float) current_time * 21.0f, 
      glm::vec3(1.0f, 0.0f, 0.0f)
    );

    glUniformMatrix4fv(mv_location, 1, GL_FALSE, glm::value_ptr(mv_matrix));
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
#else
  float f = (float)current_time * 0.3f;

  glm::mat4 translation_matrix = 
    glm::translate(
      glm::mat4(),
      glm::vec3(
        sinf(2.1f * f) * 0.5f,
        cosf(1.7f * f) * 0.5f,
        sinf(1.3f * f) * cosf(1.5f * f) * 2.0f
      )
    );
  
  glm::mat4 rotation_matrix = 
    glm::rotate(glm::mat4(), (float)current_time * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
    glm::rotate(glm::mat4(), (float)current_time * 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 scale_matrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));

  glm::mat4 model_matrix = translation_matrix * rotation_matrix * scale_matrix * identity_matrix;


  // NOTE
  // glm supports chaining matrices for exactly this kind of situation
  // and it is optimized to do so
  // using glm::transform(glm::mat4()...) is just for explicit purposes :)
  /*glm::mat4 view_translation_matrix = glm::translate(
    view_matrix, 
    glm::vec3(
      sinf(2.1f * f) * 0.5f,
      cosf(1.7f * f) * 0.5f,
      sinf(1.3f * f) * cosf(1.5f * f) * 2.0f
    )
  );

  glm::mat4 rotated_vt_matrix = glm::rotate(
    view_translation_matrix, 
    (float) current_time * 45.0f,
    glm::vec3(0.0f, 1.0f, 0.0f)
  );

  glm::mat4 mv_matrix = glm::rotate(
    rotated_vt_matrix,
    (float) current_time * 81.0f,
    glm::vec3(1.0f, 0.0f, 0.0f)
  );*/


  glm::mat4 mv_matrix = view_matrix * model_matrix;  
  glUniformMatrix4fv(mv_location, 1, GL_FALSE, glm::value_ptr(mv_matrix));

  glDrawArrays(GL_TRIANGLES, 0, 36);
#endif
}

void SpinningCube::on_key(int key, int action)
{
}

void SpinningCube::on_resize(int w, int h)
{
  width = w;
  height = h;
  aspect = (float) w / (float) h;
  proj_matrix = glm::perspective(50.0f, aspect, 0.1f, 1000.0f);
  printf("SpinningCube::on_resize(%d, %d)\n", width, height);
}

void SpinningCube::on_mouse(int button, int action) 
{
}

void SpinningCube::handle_click() 
{
}

void SpinningCube::shutdown()
{
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

}

int main(int argc, char* args[]) 
{
  chapter5::SpinningCube* app = new chapter5::SpinningCube;
  app->run(app);

  return 0;
}
