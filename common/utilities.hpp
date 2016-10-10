#ifndef JOHN_UTILITIES_H
#define JOHN_UTILITIES_H

namespace john 
{

namespace utilities
{
  int compile_shaders(const char* vtx_shdr_txt, const char* frg_shdr_txt) {
    GLuint vtx_shader, frg_shader, programID;
    vtx_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtx_shader, 1,&vtx_shdr_txt, NULL);
    glCompileShader(vtx_shader);

    frg_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frg_shader, 1, &frg_shdr_txt, NULL);
    glCompileShader(frg_shader);
    programID = glCreateProgram();
    glAttachShader(programID, vtx_shader);
    glAttachShader(programID, frg_shader);
    glLinkProgram(programID);

    printf("vertex shader id: %d\n", vtx_shader);
    printf("fragment shader id: %d\n", frg_shader);

    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);

    if (0 == programID) { 
      printf("Error compiling shaders: %d\n", programID);
    }

    return programID;

  }

  int compile_shaders(const char* vtx_shdr_txt, const char* frg_shdr_txt, const char* tess_ctrl_txt, const char* tess_eval_txt, const char* geom_shader_txt) {
    GLuint vtx_shader, frg_shader, tess_ctrl, tess_eval, geom_shader, programID;
    vtx_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vtx_shader, 1,&vtx_shdr_txt, NULL);
    glCompileShader(vtx_shader);

    frg_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frg_shader, 1, &frg_shdr_txt, NULL);
    glCompileShader(frg_shader);

    tess_ctrl = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tess_ctrl, 1, &tess_ctrl_txt, NULL);
    glCompileShader(tess_ctrl);

    tess_eval = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tess_eval, 1, &tess_eval_txt, NULL);
    glCompileShader(tess_eval);

    geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geom_shader, 1, &geom_shader_txt, NULL);
    glCompileShader(geom_shader);

    programID = glCreateProgram();
    glAttachShader(programID, vtx_shader);
    glAttachShader(programID, tess_ctrl);
    glAttachShader(programID, tess_eval);
    glAttachShader(programID, frg_shader);
    glAttachShader(programID, geom_shader);
    glLinkProgram(programID);

    printf("vertex shader id: %d\n", vtx_shader);
    printf("fragment shader id: %d\n", frg_shader);
    printf("tess_eval id: %d\n", tess_eval);
    printf("tess_ctrl id: %d\n", tess_ctrl);
    printf("geom_shader id: %d\n", geom_shader);

    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);
    glDeleteShader(tess_ctrl);
    glDeleteShader(tess_eval);
    glDeleteShader(geom_shader);

    if (0 == programID) { 
      printf("Error compiling shaders: %d\n", programID);
    }

    return programID;
  }
}

}

#endif
