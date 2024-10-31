#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "../gls/mesh.h"
#include "../gls/shader.h"
#include <condition_variable>
#include <mutex>
#include <string>
#ifdef __APPLE__
#include <vector>
#endif //__APPLE__

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
struct triangle {
  vertex *v1, *v2, *v3;
  unsigned int e1, e2, e3;
};

class glwindow {
  // 静态回调函数
  static void onresize(GLFWwindow *window, int w, int h);
  static void onkeyinput(GLFWwindow *window, int key, int keycode, int action,
                         int mods);
  static void onmouseinput(GLFWwindow *window, int button, int action,
                           int mods);
  static void onmousemove(GLFWwindow *window, double x, double y);
  static void onmousewheel(GLFWwindow *window, double dx, double dy);

#ifdef __unix
  // 线程锁
  std::mutex mtx;
  // 条件变量
  std::condition_variable cv;
  void render();
#endif //__unix

#ifdef __APPLE__
  // 线程锁
  static std::mutex mtx;
  // 条件变量
  static std::condition_variable cv;
  static std::vector<glwindow *> windows;

  // 查询是否有需要渲染的窗口
  static bool shoud_render();

public:
  // macos特有的主线程阻塞渲染
  static void start_render();
#endif //__APPLE__

protected:
  // 可覆写窗口事件
  virtual void resize_event(resizeevent event);
  virtual void key_event(keyevent event);
  virtual void mouse_event(mouseevent event);
  virtual void mouse_move_event(mousemoveevent event);
  virtual void mouse_wheel_event(mousewheelevent event);

  // 绘制窗口
  virtual void draw_frame();

public:
  // 初始化窗口
  glwindow(int w, int h, std::string title);
  virtual ~glwindow();

  void enable_alpha_blend();

  void hide();

  void set_visible(bool val);
  void set_background_color(float *color);

  // glfw是否初始化完成
  static bool _is_glfw_inited;
  // glew是否初始化完成
  static bool _is_glew_inited;
  // 窗口指针
  GLFWwindow *window;
  // 是否显示窗口
  bool visible{false};
  bool gl_visible{true};
  bool delay_visible{false};
  // 着色器
  shader *_shader;
  // 图形数据
  mesh *_mesh;
  // 背景颜色
  float background_color[4]{0.23f, 0.23f, 0.23f, 1.0f};
#ifdef __unix
  std::thread _eventloop_thread;
#endif //__unix
};

#endif // GLWINDOW_H
