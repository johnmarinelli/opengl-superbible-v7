#include "glengine/application.hpp"
#include <math.h>

namespace chapter3
{

const char* vertex_glsl = 
"#version 410 core                                                 \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    const vec4 vertices[] = vec4[](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
"                                   vec4(-0.25, -0.25, 0.5, 1.0),  \n"
"                                   vec4(-0.25, 0.25, 0.5, 1.0),  \n"
"                                   vec4( 0.25,  0.25, 0.5, 1.0)); \n"
"                                                                  \n"
"    gl_Position = vertices[gl_VertexID];                          \n"
"}                                                                 \n";

const char* fragment_glsl = 
"#version 410 core                                                 \n"
"                                                                  \n"
"out vec4 color;                                                   \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(0.0, 0.8, 1.0, 1.0);                             \n"
"}                                                                 \n";

const char* tess_ctrl_glsl = 
"#version 410 core\n"
"\n"
"layout (vertices = 3) out;\n"
"\n"
"void main(void)\n"
"{\n"
"    if (gl_InvocationID == 0)\n"
"    {\n"
"        gl_TessLevelInner[0] = 3.0;\n"
"        gl_TessLevelOuter[0] = 4.0;\n"
"        gl_TessLevelOuter[1] = 5.0;\n"
"        gl_TessLevelOuter[2] = 6.0;\n"
"    }\n"
"    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
"}\n";

const char* tess_eval_glsl = 
"#version 410 core                                                                 \n"
"                                                                                  \n"
"layout (triangles, equal_spacing, cw) in;                                         \n"
"                                                                                  \n"
"void main(void)                                                                   \n"
"{                                                                                 \n"
"    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) +                       \n"
"                  (gl_TessCoord.y * gl_in[1].gl_Position) +                       \n"
"                  (gl_TessCoord.z * gl_in[2].gl_Position);                        \n"
"}                                                                                 \n";

class Tesselation : public john::Application
{
public:
  Tesselation();

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

Tesselation::Tesselation() : 
  john::Application(),
  program_id(0)
{
}

void Tesselation::startup()
{
  john::Application::startup();

  program_id = compile_shaders(vertex_glsl, fragment_glsl, tess_ctrl_glsl, tess_eval_glsl);
  // draw only outlines of resulting triangles.  wireframe mode == GL_LINE
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // tell opengl that every patch has 4 verts
  glPatchParameteri(GL_PATCH_VERTICES, 4);
  printf("program id: %d\n", program_id);
}

void Tesselation::render(double current_time)
{
  const GLfloat bg_color[] = { 
    0.0,
    0.0,
    0.0,
    1.0 
  };

  glClearBufferfv(GL_COLOR, 0, bg_color);

  glUseProgram(program_id);
  glDrawArrays(GL_PATCHES, 0, 4);
}

void Tesselation::on_key(int key, int action)
{
}

void Tesselation::on_mouse(int button, int action) 
{
}

void Tesselation::handle_click() 
{
}

void Tesselation::shutdown()
{
}

}

int main(int argc, char* args[]) 
{
  chapter3::Tesselation* app = new chapter3::Tesselation;
  app->run(app);

  return 0;
}
