#include "glwindow.h"
#include "../gls/glcore.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

bool glwindow::_is_glfw_inited = false;
bool glwindow::_is_glew_inited = false;

#ifdef __APPLE__
std::vector<glwindow *> glwindow::windows;
std::mutex glwindow::mtx;
std::condition_variable glwindow::cv;
#endif //__APPLE__

void glwindow::onresize(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);
  resizeevent event;
  event.window = window;
  event.w = w;
  event.h = h;
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 发送事件
    instance->resize_event(event);
  }
};

void glwindow::onkeyinput(GLFWwindow *window, int key, int keycode, int action,
                          int mods) {
  keyevent event;
  event.window = window;
  event.key = key;
  event.keycode = keycode;
  event.action = action;
  event.mods = mods;
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 发送事件
    instance->key_event(event);
  }
};

void glwindow::onmouseinput(GLFWwindow *window, int button, int action,
                            int mods) {
  mouseevent event;
  event.window = window;
  event.button = button;
  event.action = action;
  event.mods = mods;
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 发送事件
    instance->mouse_event(event);
  }
};

void glwindow::onmousemove(GLFWwindow *window, double x, double y) {
  mousemoveevent event;
  event.window = window;
  event.x = x;
  event.y = y;
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 发送事件
    instance->mouse_move_event(event);
  }
};
void glwindow::onmousewheel(GLFWwindow *window, double dx, double dy) {
  mousewheelevent event;
  event.window = window;
  event.dx = dx;
  event.dy = dy;
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 发送事件
    instance->mouse_wheel_event(event);
  }
};
// macos使用主线程调用死循环渲染
#ifdef __APPLE__
void glwindow::start_render() {
  while (true) {
    std::unique_lock<std::mutex> lock(mtx);
    // std::cout << "等待渲染" << std::endl;
    //    等待渲染窗口
    cv.wait(lock, []() { return shoud_render(); });
    for (auto w : windows) {
      // 防死机机制
      if ((w->visible || w->delay_visible) &&
          !glfwWindowShouldClose(w->window)) {
        // 更改窗口可见性
        if (!w->visible) {
          glfwHideWindow(w->window);
          w->gl_visible = false;
        } else if (!w->gl_visible) {
          glfwShowWindow(w->window);
          w->gl_visible = true;
        }
        // std::cout << "开始渲染" << std::endl;
        glfwMakeContextCurrent(w->window);
        // 绘制背景颜色
        glClearColor(w->background_color[0], w->background_color[1],
                     w->background_color[2], w->background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);
        w->_shader->use();
        w->_mesh->bind();
        // 绘制内容
        w->draw_frame();
        w->_mesh->unbind();
        w->_shader->unuse();

        glfwSwapBuffers(w->window);
      } else {
        glfwHideWindow(w->window);
        w->gl_visible = false;
        std::thread t([&]() {
          // 防止未响应
          usleep(100000);
          w->set_visible(false);
        });
        t.detach();
      }
      glfwPollEvents();
    }
  }
}

// 检查是否需要渲染窗口
bool glwindow::shoud_render() {
  for (auto w : windows) {
    if (w->visible || w->delay_visible) {
      return true;
    }
  }
  return false;
}
#endif //__APPLE__

void glwindow::hide() { glfwHideWindow(window); }

// 构造函数
glwindow::glwindow(int w, int h, std::string title) {
  if (!_is_glfw_inited) {
    // 初始化glfw,OpenGL
    int fwiret = glfwInit();
    if (!fwiret) {
      throw std::runtime_error("GLFW初始化失败");
    } else {
      _is_glfw_inited = true;
    }
    // 版本号请求
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 使用opengl核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 前向适配macos平台opengl legacy
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //__APPLE__
  }
  // 初始化窗口
  window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
  // 设置绑定当前实例为用户
  glfwSetWindowUserPointer(window, this);

  if (!_is_glew_inited) {
    // 初始化glew前需要激活opengl,设置opengl上下文
    glfwMakeContextCurrent(window);
    // 初始化glew(初始化opengl函数)
    int ewiret = glewInit();
    if (ewiret) {
      throw std::runtime_error("GLEW初始化失败");
    } else {
      _is_glew_inited = true;
    }
  }
  // 初始化shader
  _shader = new shader("../assets/shader/vertex.glsl",
                       "../assets/shader/fragment.glsl");
  // 初始化mesh
  _mesh = new mesh();
  // 绑定回调函数
  glfwSetFramebufferSizeCallback(window, onresize);
  glfwSetKeyCallback(window, onkeyinput);
  glfwSetMouseButtonCallback(window, onmouseinput);
  glfwSetCursorPosCallback(window, onmousemove);
  glfwSetScrollCallback(window, onmousewheel);

#ifdef __APPLE__
  windows.push_back(this);
#endif //__APPLE__
  // linux使用多线程
#ifdef __unix
  // linux需要从主线程释放gl上下文以便各线程绑定渲染
  glfwMakeContextCurrent(nullptr);
  _eventloop_thread = std::thread(&glwindow::render, this);
  std::cout << "启动渲染线程" << std::endl;
  _eventloop_thread.detach();
#endif //__unix
}

// 析构函数
glwindow::~glwindow() {
  glfwDestroyWindow(window);
  delete _shader;
  delete _mesh;
}
void glwindow::set_visible(bool val) {
  // std::cout << "设置可见性[" + std::to_string(val) + "]" << std::endl;
  visible = val;
  std::thread t([this]() {
    usleep(100000);
    delay_visible = visible;
  });
  t.detach();
  // 通知线程
  cv.notify_all();
}

void glwindow::enable_alpha_blend() {
  if (!_is_glfw_inited || !_is_glew_inited) {
    throw std::runtime_error("FW未初始化");
  }
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// linux线程函数
#ifdef __unix
void glwindow::render() {
  while (true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return visible; });
    glfwMakeContextCurrent(window);
    while (visible && !glfwWindowShouldClose(window)) {
      // 绘制背景颜色
      glClearColor(background_color[0], background_color[1],
                   background_color[2], background_color[3]);
      glClear(GL_COLOR_BUFFER_BIT);

      _shader->use();
      _mesh->bind();
      // 绘制内容
      draw_frame();

      _mesh->unbind();
      _shader->unuse();

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
    // 关闭窗口后再隐藏一次窗口并完成一次事件循环保证生效
    glfwHideWindow(window);
    glfwPollEvents();
    glfwMakeContextCurrent(nullptr);
  }
};
void glwindow::getglcontext() { glfwMakeContextCurrent(window); };
void glwindow::releaseglcontext() { glfwMakeContextCurrent(nullptr); };
#endif //__unix

void glwindow::draw_frame() {}

void glwindow::resize_event(resizeevent event) {}
void glwindow::key_event(keyevent event) {}
void glwindow::mouse_event(mouseevent event) {}
void glwindow::mouse_move_event(mousemoveevent event) {}
void glwindow::mouse_wheel_event(mousewheelevent event) {}
