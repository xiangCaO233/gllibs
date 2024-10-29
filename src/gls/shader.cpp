#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

shader::shader(const char *vsourcefile, const char *fsourcefile) {
  std::fstream vsfile, fsfile;
  // 打开文件
  vsfile.open(vsourcefile);
  fsfile.open(fsourcefile);

  std::stringstream vsstream, fsstream;

  // 读取文件
  vsstream << vsfile.rdbuf();
  fsstream << fsfile.rdbuf();

  std::string vsource_str, fsource_str;
  vsource_str = vsstream.str();
  fsource_str = fsstream.str();

  // c 风格字符串
  const char *vertex_source, *fragment_source;
  vertex_source = vsource_str.c_str();
  fragment_source = fsource_str.c_str();

  // 初始化着色器
  GLuint vshader, fshader;
  vshader = glCreateShader(GL_VERTEX_SHADER);
  fshader = glCreateShader(GL_FRAGMENT_SHADER);

  // 注入源代码
  glShaderSource(vshader, 1, &vertex_source, nullptr);
  glShaderSource(fshader, 1, &fragment_source, nullptr);

  // 编译着色器
  glCompileShader(vshader);
  // 检查编译错误
  check_error(vshader, Type::VERTEX);
  glCompileShader(fshader);
  check_error(fshader, Type::FRAGMENT);

  // 初始化着色器程序
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vshader);
  glAttachShader(shader_program, fshader);
  // 链接着色器
  glLinkProgram(shader_program);
  check_error(shader_program, Type::PROGRAM);

  // 释放临时着色器
  glDeleteShader(vshader);
  glDeleteShader(fshader);
};

// 析构对象
shader::~shader() { glDeleteProgram(shader_program); };

void shader::check_error(GLuint val, Type t) {
  int success;
  char log[512];
  switch (t) {
  case VERTEX: {
    glGetShaderiv(val, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(val, 512, nullptr, log);
      std::cout << "顶点着色器编译失败:" << log << std::endl;
    }
    break;
  }
  case FRAGMENT: {
    glGetShaderiv(val, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(val, 512, nullptr, log);
      std::cout << "片段着色器编译失败:" << log << std::endl;
    }
    break;
  }
  case PROGRAM: {
    glGetProgramiv(val, GL_LINK_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(val, 512, nullptr, log);
      std::cout << "着色器链接失败:" << log << std::endl;
    }
    break;
  }
  }
}

void shader::use() { glUseProgram(shader_program); };

void shader::unuse() { glUseProgram(0); };
