#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <vector>

class vertex {

public:
  // 标准数据
  float _x{0.0f}, _y{0.0f}, _z{0.0f}, _r{0.0f}, _g{0.0f}, _b{0.0f}, _a{0.0f},
      _u{0.0f}, _v{0.0f};
  // 初始化顶点对象
  vertex(float x, float y, float z, float r, float g, float b, float a, float u,
         float v);
  vertex(float x, float y, float z, float r, float g, float b, float a);
  vertex(float x, float y, float z, float u, float v);
  vertex(float x, float y, float z);
  ~vertex();

  std::vector<float> dump();

  void register_data(std::string name, float val);

  bool operator==(const vertex &v);
};

#endif // VERTEX_H
