#include "glwindow.h"
#include <stdexcept>

bool glwindow::_is_glfw_inited = false;
bool glwindow::_is_glew_inited = false;

void glwindow::onresize(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);
  resizeevent *event;
  event->window = window;
  event->w = w;
  event->h = h;
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
  keyevent *event;
  event->window = window;
  event->key = key;
  event->keycode = keycode;
  event->action = action;
  event->mods = mods;
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
  mouseevent *event;
  event->window = window;
  event->button = button;
  event->action = action;
  event->mods = mods;
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 发送事件
    instance->mouse_event(event);
  }
};

void glwindow::onmousemove(GLFWwindow *window, double x, double y) {
  mousemoveevent *event;
  event->window = window;
  event->x = x;
  event->y = y;
  // 获取用户实例指针
  glwindow *instance =
      static_cast<glwindow *>(glfwGetWindowUserPointer(window));
  if (instance) {
    // 发送事件
    instance->mouse_move_event(event);
  }
};
void glwindow::onmousewheel(GLFWwindow *window, double dx, double dy) {
  mousewheelevent *event;
  event->window = window;
  event->dx = dx;
  event->dy = dy;
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
    for (auto w : windows) {
      if (w->visible) {
        // 获取用户实例指针
        glwindow *instance =
            static_cast<glwindow *>(glfwGetWindowUserPointer(window));
        if (instance) {
          glfwMakeContextCurrent(window);
          // 绘制窗口
          glwindow->draw_frame();
        }
      }
    }
  }
}
#endif //__APPLE__

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
  windows.push_back(window);
#endif //__APPLE__
  // linux使用多线程
#ifdef __unix
  _eventloop_thread = std::thread(&glwindow::render, this);
  _eventloop_thread.detach();
#endif //__unix
}

// 析构函数
glwindow::~glwindow() {
  delete _shader;
  delete _mesh;
}
void glwindow::set_visible(bool val) { visible = val; }

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
  while (visible) {
    //  glfwMakeContextCurrent(window);
    // 绘制背景颜色
    glClearColor(background_color[0], background_color[1], background_color[2],
                 background_color[3]);
    // glClear(GL_COLOR_BUFFER_BIT);

    //_shader->use();
    //_mesh->bind();
    // 绘制内容
    // draw_frame();

    //_mesh->unbind();
    //_shader->unuse();

    // glfwSwapBuffers(window);
    // glfwPollEvents();
  }
};
#endif //__unix

void glwindow::draw_frame() {}

void glwindow::resize_event(resizeevent *event) {}
void glwindow::key_event(keyevent *event) {}
void glwindow::mouse_event(mouseevent *event) {}
void glwindow::mouse_move_event(mousemoveevent *event) {}
void glwindow::mouse_wheel_event(mousewheelevent *event) {}
