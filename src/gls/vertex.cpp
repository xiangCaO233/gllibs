#include "vertex.h"
#include <vector>

vertex::vertex(float x, float y, float z, float r, float g, float b, float a,
               float u, float v)
    : _x(x), _y(y), _z(z), _r(r), _g(g), _b(b), _a(a), _u(u), _v(v){};
vertex::vertex(float x, float y, float z, float r, float g, float b, float a)
    : _x(x), _y(y), _z(z), _r(r), _g(g), _b(b), _a(a){};
vertex::vertex(float x, float y, float z, float u, float v)
    : _x(x), _y(y), _z(z), _u(u), _v(v){};
vertex::vertex(float x, float y, float z) : _x(x), _y(y), _z(z){};

vertex::~vertex() {}

std::vector<float> vertex::dump() {
  std::vector<float> standard_data = {_x, _y, _z, _r, _g, _b, _a, _u, _v};
  return standard_data;
}

bool vertex::operator==(const vertex &v) {
  return _x == v._x && _y == v._y && _z == v._z && _r == v._r && _g == v._g &&
         _b == v._b && _a == v._a && _u == v._u && _v == v._v;
}
