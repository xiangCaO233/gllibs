#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "../gls/mesh.h"
#include "../gls/shader.h"
#include <string>
#include <vector>

#ifdef __unix
#include <thread>
#endif //__unix

// 打包事件发送到实例
struct resizeevent {
  GLFWwindow *window;
  int w, h;
};
struct keyevent {
  GLFWwindow *window;
  int key, keycode, action, mods;
};
struct mouseevent {
  GLFWwindow *window;
  int button, action, mods;
};
struct mousemoveevent {
  GLFWwindow *window;
  double x, y;
};
struct mousewheelevent {
  GLFWwindow *window;
  double dx, dy;
};

class glwindow {
#ifdef __APPLE__
  static std::vector<glwindow *> windows;
  static void start_render();
#endif //__APPLE__
  // 静态回调函数
  static void onresize(GLFWwindow *window, int w, int h);
  static void onkeyinput(GLFWwindow *window, int key, int keycode, int action,
                         int mods);
  static void onmouseinput(GLFWwindow *window, int button, int action,
                           int mods);
  static void onmousemove(GLFWwindow *window, double x, double y);
  static void onmousewheel(GLFWwindow *window, double dx, double dy);

protected:
  // 可覆写窗口事件
  virtual void resize_event(resizeevent *event);
  virtual void key_event(keyevent *event);
  virtual void mouse_event(mouseevent *event);
  virtual void mouse_move_event(mousemoveevent *event);
  virtual void mouse_wheel_event(mousewheelevent *event);

  virtual void drawframe();

public:
  // 初始化一个窗口
  glwindow(int w, int h, std::string title);
  ~glwindow();

  void enable_alpha_blend();

  // glfw是否初始化完成
  static bool _is_glfw_inited;
  // glew是否初始化完成
  static bool _is_glew_inited;
  // 窗口指针
  GLFWwindow *window;
  // 着色器
  shader _shader;
  // 图形数据
  mesh _mesh;
#ifdef __unix
  std::thread _eventloop_thread;
#endif //__unix
};

#endif // GLWINDOW_H