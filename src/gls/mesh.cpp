#include "mesh.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
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
void mesh::put_vertex(vertex &v, int index) {
  // debug_vertex_data();
  auto vdata = v.dump();
  if (_vertices.size() >= _vsize - 1) {
    // 括申显存
    expand_gmemory(GL_ARRAY_BUFFER, 1.5f);
  }
  bind();
  // 更新显存
  if (index == -1) {
    // 添加到末尾
    glBufferSubData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(vertex),
                    sizeof(vertex), vdata.data());
    // 同步cpu侧数据
    std::copy(vdata.begin(), vdata.end(),
              _vertices_data.begin() + _vertices.size() * 9);
    _vertices.push_back(v);
  } else if (index >= 0 && index < _vertices.size()) {
    // 添加到指定位置
    glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex), sizeof(vertex),
                    vdata.data());
    // 同步cpu侧数据
    std::copy(vdata.begin(), vdata.end(), _vertices_data.begin() + index * 9);
    _vertices[index] = v;
  } else {
    throw std::runtime_error("index非法,顶点插入失败");
  }
  unbind();
  // debug_vertex_data();
  // debug_gmemory();
};
void mesh::put_vertex(vertex &&v, int index) { put_vertex(v, index); };

void mesh::put_vertices(std::vector<vertex> &vs, int index) {
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
  if (index == -1) {
    // 更新显存
    glBufferSubData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(vertex),
                    vs.size() * sizeof(vertex), temp.data());
    // 同步cpu侧数据
    std::copy(temp.begin(), temp.end(),
              _vertices_data.begin() + _vertices.size() * 9);
    _vertices.insert(_vertices.end(), vs.begin(), vs.end());
  } else if (index >= 0 && index < _vertices.size()) {
    // 更新显存
    glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex),
                    vs.size() * sizeof(vertex), temp.data());
    // 同步cpu侧数据
    std::copy(temp.begin(), temp.end(),
              _vertices_data.begin() + _vertices.size() * 9);
    _vertices.insert(_vertices.end(), vs.begin(), vs.end());
  } else {
    throw std::runtime_error("index非法,顶点插入失败");
  }
  unbind();
}

void mesh::put_vertices(std::vector<vertex> &&vs, int index) {
  put_vertices(vs, index);
}

// 更新顶点数据
void mesh::update_vertex_coord(int index, float x, float y, float z) {
  if (index < 0 || index >= _vertices.size())
    throw std::runtime_error("下标非法,修改失败");
  bind();
  // 修改显存
  std::vector<float> data = {x, y, z};
  glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex) + 0,
                  3 * sizeof(float), data.data());
  // 同步cpu侧数据
  _vertices[index]._x = x;
  _vertices[index]._y = y;
  _vertices[index]._z = z;
  _vertices_data[index * 9] = x;
  _vertices_data[index * 9 + 1] = y;
  _vertices_data[index * 9 + 2] = z;
  unbind();

  // debug_gmemory();
};
void mesh::update_vertex_rcoord(int index, float rx, float ry, float rz) {
  update_vertex_coord(index, _vertices[index]._x + rx, _vertices[index]._y + ry,
                      _vertices[index]._z + rz);
}
void mesh::update_vertex_color(int index, float r, float g, float b, float a) {
  if (index < 0 || index >= _vertices.size())
    throw std::runtime_error("下标非法,修改失败");
  bind();
  // 修改显存
  std::vector<float> data = {r, g, b, a};
  glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex) + 3 * sizeof(float),
                  4 * sizeof(float), data.data());
  // 同步cpu侧数据
  _vertices[index]._r = r;
  _vertices[index]._g = g;
  _vertices[index]._b = b;
  _vertices[index]._a = a;
  _vertices_data[index * 9 + 3] = r;
  _vertices_data[index * 9 + 4] = g;
  _vertices_data[index * 9 + 6] = b;
  _vertices_data[index * 9 + 7] = a;
  unbind();
};
void mesh::update_vertex_rcolor(int index, float rr, float rg, float rb,
                                float ra) {
  update_vertex_color(index, _vertices[index]._r + rr, _vertices[index]._g + rg,
                      _vertices[index]._b + rb, _vertices[index]._a + ra);
};
void mesh::update_vertex_uv(int index, float u, float v) {
  if (index < 0 || index >= _vertices.size())
    throw std::runtime_error("下标非法,修改失败");
  bind();
  // 修改显存
  std::vector<float> data = {u, v};
  glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex) + 7 * sizeof(float),
                  2 * sizeof(float), data.data());
  // 同步cpu侧数据
  _vertices[index]._u = u;
  _vertices[index]._v = v;
  _vertices_data[index * 9 + 7] = u;
  _vertices_data[index * 9 + 8] = v;
  unbind();
};

void mesh::update_vertex_ruv(int index, float ru, float rv) {
  update_vertex_uv(index, _vertices[index]._u + ru, _vertices[index]._v + rv);
};
void mesh::del_vertex(int index, bool align_mem, bool auto_elm_fix) {
  if (index < 0 || index >= _vertices.size()) {
    throw std::runtime_error("index非法,顶点删除失败");
  }
  bind();
  if (align_mem) {
    // 对齐内存(会影响element,会导致绘制失效,需要实时变化element数组)
    // 处理显存
    // 获取此顶点后面的数据
    int after_datasize = (_vertices.size() - index - 1) * sizeof(vertex);
    // 申请一片后面数据大小的内存
    std::vector<char> after_data(after_datasize);
    glGetBufferSubData(GL_ARRAY_BUFFER, (index + 1) * sizeof(vertex),
                       after_datasize, after_data.data());
    // 覆盖整个后部分数据
    glBufferSubData(GL_ARRAY_BUFFER, (index) * sizeof(vertex), after_datasize,
                    after_data.data());
    // 将显存最后顶点内存置零
    auto blank_gmem = std::vector<float>(9, 0.0f);
    glBufferSubData(GL_ARRAY_BUFFER, (_vertices.size() - 1) * sizeof(vertex),
                    sizeof(vertex), blank_gmem.data());

    if (auto_elm_fix) {
      // 修复element数组下标
      // 将所有等于 index 的元素移到_elements末尾
      auto _elements_new_end =
          std::remove(_elements.begin(), _elements.end(), index);
      // 删除的元素数量
      int num_removed = std::distance(_elements_new_end, _elements.end());
      // 从新结尾删除无效数据
      _elements.erase(_elements_new_end, _elements.end());
      // 同步cpu侧_element_data
      auto _element_data_new_end =
          std::remove(_element_data.begin(), _element_data.end(), index);
      _element_data.erase(_element_data_new_end, _element_data.end());

      if (auto_elm_fix) {
        // 修复_elements和_element_data数组
        auto fix_elements = [&](std::vector<unsigned int> &elements) {
          auto new_end = std::remove(elements.begin(), elements.end(), index);
          elements.erase(new_end, elements.end());
          for (auto &ele : elements) {
            if (ele > index)
              --ele;
          }
        };
        fix_elements(_elements);
        fix_elements(_element_data);

        // 同步显存中的elements数据
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                        _element_data.size() * sizeof(unsigned int),
                        _element_data.data());
      }

      // cpu侧
      // 擦除顶点列表中的数据
      _vertices.erase(_vertices.begin() + index);

      // 擦除顶点数据中的数据
      _vertices_data.erase(_vertices_data.begin() + index * 9,
                           _vertices_data.begin() + (index + 1) * 9);
      auto blank_data = std::vector<float>(9, 0.0f);
      // 补齐空白数据到末尾
      _vertices_data.insert(_vertices_data.end(), blank_data.begin(),
                            blank_data.end());
    } else {
      // 不对齐内存(不影响element主要绘制逻辑)
      auto blank_data = std::vector<float>(9, 0.0f);
      // 不擦除顶点列表中的数据
      // 将顶点数据中的数据置零
      std::copy(blank_data.begin(), blank_data.end(),
                _vertices_data.begin() + index * 9);
      // 处理显存
      auto blank_gmem = std::vector<float>(9, 0.0f);
      // 将指定位置的顶点数据置零
      glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(vertex), sizeof(vertex),
                      blank_gmem.data());
    }
    unbind();
  }
}

void mesh::del_vertex(vertex &v, bool align_mem, bool auto_elm_fix) {
  auto it = std::find(_vertices.begin(), _vertices.end(), v);
  if (it != _vertices.end()) {
    del_vertex(std::distance(_vertices.begin(), it), align_mem, auto_elm_fix);
  }
}

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
void mesh::add_elements(std::vector<unsigned int> &&es) {
  // debug_gememory();
  // debug_element_data();
  if (es.size() + _elements.size() > _esize - 1) {
    // 超出容量
    float rate =
        (float)(es.size() + _elements.size()) / (float)(_elements.size()) +
        0.5f;
    expand_gmemory(GL_ELEMENT_ARRAY_BUFFER, rate);
  }
  bind();
  // 更新显存
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                  _elements.size() * sizeof(unsigned int),
                  es.size() * sizeof(unsigned int), es.data());
  unbind();
  // 同步cpu侧数据
  std::copy(es.begin(), es.end(), _element_data.begin() + _elements.size());
  _elements.insert(_elements.end(), es.begin(), es.end());
  // debug_element_data();
  // debug_gememory();
};

void mesh::insert_element(int index, unsigned int e) {
  if (_elements.size() >= _esize - 1) {
    // 括申显存
    expand_gmemory(GL_ELEMENT_ARRAY_BUFFER, 1.5f);
  }
  // 同步cpu侧数据
  _element_data.insert(_element_data.begin() + index, e);
  _elements.insert(_elements.begin() + index, e);
  // 移动显存后续数据
  bind();
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index * sizeof(unsigned int),
                  (_element_data.size() - index) * sizeof(unsigned int),
                  _element_data.data() + index);
  unbind();
};
void mesh::insert_elements(int index, std::vector<unsigned int> &&es) {
  if (es.size() + _elements.size() > _esize - 1) {
    // 超出容量
    float rate =
        (float)(es.size() + _elements.size()) / (float)(_elements.size()) +
        0.5f;
    expand_gmemory(GL_ELEMENT_ARRAY_BUFFER, rate);
  }
  // 同步cpu侧数据

  _element_data.insert(_element_data.begin() + index, es.begin(), es.end());
  for (int i = 0; i < es.size(); i++)
    _element_data.pop_back();
  _elements.insert(_elements.end(), es.begin(), es.end());
  bind();
  // 更新显存
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index * sizeof(unsigned int),
                  (_element_data.size() - index) * sizeof(unsigned int),
                  _element_data.data() + index);
  unbind();
};
void mesh::insert_elements(int index, std::vector<unsigned int> &es) {
  insert_elements(index, es);
};

void mesh::del_element(int index) {
  if (index < 0 || index >= _elements.size())
    throw std::runtime_error("元素下标非法,删除失败");
  bind();
  // 处理显存数据
  int after_data_size = (_elements.size() - index - 1) * sizeof(unsigned int);
  std::vector<char> after_data(after_data_size);
  // 获取显存数据
  glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                     (index + 1) * sizeof(unsigned int), after_data_size,
                     after_data.data());
  // 更新显存
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, index * sizeof(unsigned int),
                  after_data_size, after_data.data());
  unbind();
  // 同步cpu侧数据
  _element_data.erase(_element_data.begin() + index);
  _elements.erase(_elements.begin() + index);
}

void mesh::erase_element(int from, int to) {
  if (from < 0 || to >= _elements.size() || from > to)
    throw std::runtime_error("元素下标非法,删除失败");
  // 处理显存数据
  int after_data_size = (_elements.size() - to - 1) * sizeof(unsigned int);
  std::vector<char> after_data(after_data_size);
  if (to < _elements.size() - 1) {
    bind();
    // 获取显存数据
    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (to + 1) * sizeof(unsigned int),
                       after_data_size, after_data.data());
    // 更新显存
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, from * sizeof(unsigned int),
                    after_data_size, after_data.data());
    unbind();
  }
  // 同步cpu侧数据
  _element_data.erase(_element_data.begin() + from, _element_data.begin() + to);
  _elements.erase(_elements.begin() + from, _elements.begin() + to);
}
