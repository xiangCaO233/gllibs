#include "mesh.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

mesh::mesh(GLenum mesh_type, int init_size)
    : _type(mesh_type), _vsize(init_size), _esize(init_size * 4),
      _vbuffer_size(init_size * sizeof(vertex)),
      _ebuffer_size(_esize * sizeof(unsigned int)) {
  // 生成顶点缓冲对象
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // 绑定VAO
  glBindVertexArray(VAO);

  // 初始化cpu侧buffer_data
  _vertices_data = std::vector<float>(_vsize * 9, 0.0f);

  // 绑定VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 上传初始化VBO显存数据
  glBufferData(GL_ARRAY_BUFFER, _vbuffer_size, _vertices_data.data(), _type);

  // 初始化cpu侧element_data
  _element_data = std::vector<unsigned int>(_esize, 0);
  // 绑定EBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // 上传初始化EBO显存数据
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, _ebuffer_size, _element_data.data(),
               _type);

  // 配置显存描述
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (void *)(7 * sizeof(float)));
  glEnableVertexAttribArray(2);
  unbind();
}

mesh::~mesh(){};

void mesh::debug_vertex_data() {
  std::cout << "当前顶点数据" << std::endl;
  int i = 1;
  for (float d : _vertices_data) {
    std::cout << d << ",";
    if (i % 9 == 0)
      std::cout << std::endl;
    i++;
  }
}
void mesh::debug_element_data() {
  std::cout << "当前元素数据" << std::endl;
  int i = 1;
  for (unsigned int d : _element_data) {
    std::cout << d << ",";
    if (i % 3 == 0)
      std::cout << std::endl;
    i++;
  }
}
void mesh::debug_gmemory() {
  std::cout << "当前显存数据" << std::endl;
  auto temp_vbdata = std::vector<char>(_vbuffer_size);
  bind();
  glGetBufferSubData(GL_ARRAY_BUFFER, 0, _vbuffer_size, temp_vbdata.data());
  int i = 1;
  for (char d : temp_vbdata) {
    std::cout << (int)d << ",";
    if (i % 36 == 0)
      std::cout << std::endl;
    i++;
  }
  unbind();
}
void mesh::debug_gememory() {
  std::cout << "当前显存元素数据" << std::endl;
  auto temp_ebdata = std::vector<char>(_ebuffer_size);
  bind();
  glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, _ebuffer_size,
                     temp_ebdata.data());
  int i = 1;
  for (char d : temp_ebdata) {
    std::cout << (int)d << ",";
    if (i % 4 == 0)
      std::cout << std::endl;
    i++;
  }
  unbind();
}
/**
 * 正片
 **/
void mesh::bind() {
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}
void mesh::unbind() {
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void mesh::expand_gmemory(GLenum buffer_name, float rate) {
  switch (buffer_name) {
  case GL_ARRAY_BUFFER: {
    std::cout << "VBO显存不足,正在申请扩展,倍率[" + std::to_string(rate) + "]"
              << std::endl;
    for (int i = 0; i < ((rate - 1.0f) * _vsize); i++) {
      auto extra_data = std::vector<float>(9, 0.0f);
      _vertices_data.insert(_vertices_data.end(), extra_data.begin(),
                            extra_data.end());
    }
    std::cout << "扩容完成:[" + std::to_string(_vsize) + "]~[" +
                     std::to_string((int)(_vsize * rate)) + "]"
              << std::endl;
    _vsize *= rate;
    _vbuffer_size = _vsize * sizeof(vertex);

    GLuint newVBO;
    glGenBuffers(1, &newVBO);
    glBindBuffer(GL_ARRAY_BUFFER, newVBO);
    // 上传newVBO显存数据
    glBufferData(GL_ARRAY_BUFFER, _vbuffer_size, _vertices_data.data(), _type);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // 释放原VBO
    glDeleteBuffers(1, &VBO);
    VBO = 0;
    // 更新mesh中VBO
    VBO = newVBO;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    break;
  }
  case GL_ELEMENT_ARRAY_BUFFER: {
    std::cout << "EBO显存不足,正在申请扩展,倍率[" + std::to_string(rate) + "]"
              << std::endl;
    auto extra_data =
        std::vector<unsigned int>((int)(_esize * (rate - 1.0f)), 0);
    _element_data.insert(_element_data.end(), extra_data.begin(),
                         extra_data.end());
    std::cout << "扩容完成:[" + std::to_string(_esize) + "]~[" +
                     std::to_string((int)(_esize * rate)) + "]"
              << std::endl;
    _esize *= rate;
    _ebuffer_size = _esize * sizeof(unsigned int);

    GLuint newEBO;
    glGenBuffers(1, &newEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newEBO);
    // 上传newVBO显存数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _ebuffer_size, _element_data.data(),
                 _type);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // 释放原EBO
    glDeleteBuffers(1, &EBO);
    EBO = 0;
    // 更新mesh中EBO
    EBO = newEBO;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    break;
  }
  }
}

// 添加顶点
void mesh::add_vertex(vertex &v) {
  // debug_vertex_data();
  auto vdata = v.dump();
  if (_vertices.size() >= _vsize - 1) {
    // 括申显存
    expand_gmemory(GL_ARRAY_BUFFER, 1.5f);
  }
  bind();
  // 更新显存
  glBufferSubData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(vertex),
                  sizeof(vertex), vdata.data());
  unbind();
  // 同步cpu侧数据
  std::copy(vdata.begin(), vdata.end(),
            _vertices_data.begin() + _vertices.size() * 9);
  _vertices.push_back(v);
  // debug_vertex_data();
  // debug_gmemory();
};
void mesh::add_vertex(vertex &&v) { add_vertex(v); };

void mesh::add_vertices(std::vector<vertex> &vs) {
  if (vs.size() + _vertices.size() > _vsize - 1) {
    // 超出容量
    float rate =
        (float)(vs.size() + _vertices.size()) / (float)(_vertices.size()) +
        0.5f;
    expand_gmemory(GL_ARRAY_BUFFER, rate);
  }
  std::vector<float> temp;
  for (auto vtx : vs) {
    auto data = vtx.dump();
    temp.insert(temp.end(), data.begin(), data.end());
  }
  bind();
  // 更新显存
  glBufferSubData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(vertex),
                  vs.size() * sizeof(vertex), temp.data());
  unbind();
  // 同步cpu侧数据
  std::copy(temp.begin(), temp.end(),
            _vertices_data.begin() + _vertices.size() * 9);
  _vertices.insert(_vertices.end(), vs.begin(), vs.end());
}

void mesh::add_vertices(std::vector<vertex> &&vs) { add_vertices(vs); }

// 添加顶点绘制下标
void mesh::add_element(unsigned int e) {
  // debug_gememory();
  if (_elements.size() >= _esize - 1) {
    // 括申显存
    expand_gmemory(GL_ELEMENT_ARRAY_BUFFER, 1.5f);
  }
  bind();
  // 更新显存
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                  _elements.size() * sizeof(unsigned int), sizeof(unsigned int),
                  &e);
  unbind();
  // 同步cpu侧数据
  _element_data[_elements.size()] = e;
  _elements.push_back(e);
  // debug_gememory();
};
void mesh::add_elements(std::vector<unsigned int> &&e) {
  // debug_gememory();
  // debug_element_data();
  if (e.size() + _elements.size() > _esize - 1) {
    // 超出容量
    float rate =
        (float)(e.size() + _elements.size()) / (float)(_elements.size()) + 0.5f;
    expand_gmemory(GL_ELEMENT_ARRAY_BUFFER, rate);
  }
  bind();
  // 更新显存
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                  _elements.size() * sizeof(unsigned int),
                  e.size() * sizeof(unsigned int), e.data());
  unbind();
  // 同步cpu侧数据
  std::copy(e.begin(), e.end(), _element_data.begin() + _elements.size());
  _elements.insert(_elements.end(), e.begin(), e.end());
  // debug_element_data();
  // debug_gememory();
};
