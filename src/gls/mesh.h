#ifndef MESH_H
#define MESH_H

#include "glcore.h"
#include "vertex.h"
#include <vector>

class mesh {
  void debug_gmemory();
  void debug_vertex_data();
  void debug_gememory();
  void debug_element_data();

public:
  // 缓冲数据类型
  GLenum _type;
  // 顶点数量(预设)
  int _vsize;
  // 元素数量(预设)
  int _esize;
  // 缓冲数据大小(byte)
  unsigned int _vbuffer_size;
  unsigned int _ebuffer_size;
  // 顶点缓冲对象
  // (Vertex_Buffer_Object/Vertex_Array_Object/Element_Buffer_Object)
  GLuint VBO{0}, VAO{0}, EBO{0};
  // cpu侧顶点数据
  std::vector<float> _vertices_data;
  // 元素数据
  std::vector<unsigned int> _element_data;
  // 同步顶点列表
  std::vector<vertex> _vertices;
  // 同步元素列表
  std::vector<unsigned int> _elements;

  // 构造mesh
  mesh(GLenum buffer_type = GL_STATIC_DRAW, int init_size = 10);
  ~mesh();

  // 绑定
  void bind();
  void unbind();

  // 扩展显存  buffer名  扩展倍率
  void expand_gmemory(GLenum buffer_name, float rate);

  // 添加顶点
  void add_vertex(vertex &v);
  void add_vertex(vertex &&v);
  void add_vertices(std::vector<vertex> &v);
  void add_vertices(std::vector<vertex> &&v);

  // 添加顶点绘制下标
  void add_element(unsigned int e);
  void add_elements(std::vector<unsigned int> &&e);
};

#endif // MESH_H