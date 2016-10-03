#ifndef JOHN_UTILITIES_H
#define JOHN_UTILITIES_H

namespace john 
{

namespace utilities
{

  int compile_shaders(const char* vtx_shdr_txt, const char* frg_shdr_txt, const char* tess_ctrl_txt, const char* tess_eval_txt) {
    GLuint vtx_shader, frg_shader, tess_ctrl, tess_eval, programID;
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

    programID = glCreateProgram();
    glAttachShader(programID, vtx_shader);
    glAttachShader(programID, tess_ctrl);
    glAttachShader(programID, tess_eval);
    glAttachShader(programID, frg_shader);
    glLinkProgram(programID);

    printf("vertex shader id: %d\n", vtx_shader);
    printf("fragment shader id: %d\n", frg_shader);
    printf("tess_eval id: %d\n", tess_eval);
    printf("tess_ctrl id: %d\n", tess_ctrl);

    glDeleteShader(vtx_shader);
    glDeleteShader(frg_shader);
    glDeleteShader(tess_ctrl);
    glDeleteShader(tess_eval);

    if (0 == programID) { 
      printf("Error compiling shaders: %d\n", programID);
    }

    return programID;
  }
}

}

#endif
