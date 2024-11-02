#include "nwd/glwindow.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>
#include <unistd.h>

class mywindow : public glwindow {
public:
  mywindow(int w, int h, std::string title) : glwindow(w, h, title) {}
  ~mywindow() override = default;

protected:
  void draw_frame() override {
    glDrawElements(GL_LINES, _mesh->_elements.size(), GL_UNSIGNED_INT,
                   (void *)0);
    glLineWidth(60.0f);
    // 绘制所有顶点元素(三角形方式)
    // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)0);
    _mesh->update_vertex_rcoord(0, 0.0001, 0.0001, 0);
    _mesh->update_vertex_rcoord(1, -0.0001, 0.0001, 0);
    _mesh->update_vertex_rcoord(2, -0.0001, -0.0001, 0);
    _mesh->update_vertex_rcoord(3, 0.0001, -0.0001, 0);
  }
  void resize_event(resizeevent e) override {
    std::cout << "size:[" + std::to_string(e.w) + "," + std::to_string(e.h) +
                     "]"
              << std::endl;
  }

  void key_event(keyevent e) override {
    std::cout << "key:[" + std::string(1, (char)e.key) + "],keycode:[" +
                     std::to_string(e.keycode) + "],action:[" +
                     std::to_string(e.action) + "],mods:[" +
                     std::to_string(e.mods) + "]"
              << std::endl;
  }

  void mouse_event(mouseevent e) override {
    std::cout << "button:[" + std::to_string(e.button) + "],action:[" +
                     std::to_string(e.action) + "],mods:[" +
                     std::to_string(e.mods) + "]"
              << std::endl;
  }

  void mouse_move_event(mousemoveevent e) override {
    std::cout << "mousepos:[" + std::to_string(e.x) + "," +
                     std::to_string(e.y) + "]"
              << std::endl;
  }
  void mouse_wheel_event(mousewheelevent e) override {
    std::cout << "mousepos:[" + std::to_string(e.dx) + "," +
                     std::to_string(e.dy) + "]"
              << std::endl;
  }
};

#ifdef __APPLE__
mywindow *w;
auto main_id = std::this_thread::get_id();
void moved_main() {
  std::cout << "设置窗口可见" << std::endl;
  // usleep(1000000 * 2);
  w->set_visible(true);
}
#endif //__APPLE__

int main() {
#ifdef __APPLE__
  std::cout << "创建窗口" << std::endl;
  w = new mywindow(800, 600, "test");
  // 向mesh添加顶点(x,y,z,r,g,b,a)
  w->_mesh->put_vertices({{-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
                          {1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
                          {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
                          {-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f}});
  // 添加绘制元素下标(0,1,2)前三个顶点
  w->_mesh->add_elements({0, 1, 2, 0, 2, 3});

  // 主线程必须堵塞,转移main到moved_main中运行
  std::thread t(&moved_main);
  t.detach();

  // 启动渲染循环
  glwindow::start_render();
#endif //__APPLE__

#ifdef __unix
  srand(time(nullptr));
  mywindow *w = new mywindow(800, 600, "test");
  // 获取gl上下文
  w->getglcontext();
  // 向mesh添加顶点(x,y,z,r,g,b,a)
  w->_mesh->put_vertices({
      {-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
      {1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
      {1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f},
      {-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
  });
  // 添加绘制元素下标(0,1,2)前三个顶点
  w->_mesh->add_elements({0, 1, 2, 0, 2, 3});
  // 释放gl上下文
  w->releaseglcontext();
  // 创建共享的OpenGL上下文
  GLFWwindow *sharedContext = glfwCreateWindow(1, 1, "", nullptr, w->window);
  glfwHideWindow(sharedContext);
  std::thread update_thread([&]() {
    while (true) {
      usleep(1000 * 16);
      glfwMakeContextCurrent(sharedContext);
      w->_mesh->update_vertex_color(0, (rand() % 1000) / 1000.0f,
                                    (rand() % 1000) / 1000.0f,
                                    (rand() % 1000) / 1000.0f, 1.0f);
      w->_mesh->update_vertex_color(1, (rand() % 1000) / 1000.0f,
                                    (rand() % 1000) / 1000.0f,
                                    (rand() % 1000) / 1000.0f, 1.0f);
      w->_mesh->update_vertex_color(2, (rand() % 1000) / 1000.0f,
                                    (rand() % 1000) / 1000.0f,
                                    (rand() % 1000) / 1000.0f, 1.0f);
      w->_mesh->update_vertex_color(3, (rand() % 1000) / 1000.0f,
                                    (rand() % 1000) / 1000.0f,
                                    (rand() % 1000) / 1000.0f, 1.0f);
      glfwMakeContextCurrent(nullptr);
    }
  });
  update_thread.detach();
  w->set_visible(true);
  // usleep(1000000 * 2);
  // w->set_visible(false);

  getchar();
#endif //__unix
}
