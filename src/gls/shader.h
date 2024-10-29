#ifndef SHADER_H
#define SHADER_H

#include "glcore.h"

enum Type { VERTEX, FRAGMENT, PROGRAM };

class shader {
  // 着色器程序
  GLuint shader_program;

  // 检查编译错误
  void check_error(GLuint val, Type t);

public:
  shader(const char *vsourcefile = "assets/shader/vertex.glsl",
         const char *fsourcefile = "assets/shader/fragment.glsl");
  ~shader();

  void use();
  void unuse();
};

#endif // SHADER_H
