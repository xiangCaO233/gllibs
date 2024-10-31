#include "nwd/glwindow.h"
#include <cstdio>
#include <iostream>
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
    // 绘制所有顶点元素(三角形方式)
    glDrawElements(GL_TRIANGLES, _mesh->_elements.size(), GL_UNSIGNED_INT,
                   (void *)0);
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
  // 向mesh添加三个顶点(x,y,z,r,g,b,a)
  w->_mesh->put_vertices({{-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
                          {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
                          {0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f}});
  // 添加绘制元素下标(0,1,2)前三个顶点
  w->_mesh->add_elements({0, 1, 2});

  // 主线程必须堵塞,转移main到moved_main中运行
  std::thread t(&moved_main);
  t.detach();

  // 启动渲染循环
  glwindow::start_render();
#endif //__APPLE__

#ifdef __unix
  mywindow *w = new mywindow(800, 600, "test");
  // 获取gl上下文
  w->getglcontext();
  // 向mesh添加三个顶点(x,y,z,r,g,b,a)
  w->_mesh->put_vertices({{-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
                          {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
                          {0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f}});
  // 添加绘制元素下标(0,1,2)前三个顶点
  w->_mesh->add_elements({0, 1, 2});
  // 释放gl上下文
  w->releaseglcontext();
  w->set_visible(true);
  // usleep(1000000 * 2);
  // w->set_visible(false);

  getchar();
#endif //__unix
}
