#include "nwd/glwindow.h"
#include <iostream>
#include <thread>
#include <unistd.h>

#ifdef __APPLE__
glwindow *w;

void moved_main() {

  std::cout << "设置窗口可见" << std::endl;
  usleep(1000000 * 2);
  w->set_visible(true);
  usleep(1000000 * 2);
  w->set_visible(false);
  usleep(1000000 * 2);
  w->set_visible(true);
  usleep(1000000 * 2);
  w->set_visible(false);
  usleep(1000000 * 2);
  w->set_visible(true);
  usleep(1000000 * 2);
  w->set_visible(false);
}
#endif //__APPLE__

int main() {
  // shader s("../assets/shader/vertex.glsl", "../assets/shader/fragment.glsl");
  // std::cout << "编译着色器完成" << std::endl;
  // s.use();
  // mesh ms(GL_DYNAMIC_DRAW);
  // ms.add_vertex({-0.5f, -0.5, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f});
  // ms.add_vertex({0.5f, -0.5, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f});
  // ms.add_vertex({0.0f, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f});
  // ms.add_vertex({0.5f, 0.5, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f});
  // ms.add_vertex({-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f});
  // ms.add_vertex({0.8f, 0.0, 1.0f, 0.0f, 1.0f, 0.2f, 1.0f, 0.0f, 0.0f});
  // ms.add_vertex({0.0f, 0.7, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f});
  // ms.add_vertex({0.2f, -0.3, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f});
  // ms.add_vertex({-0.8f, -0.5, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f});
  // ms.add_vertex({0.2f, 0.5, 1.0f, 1.0f, 1.0f, 0.1f, 1.0f, 0.0f, 0.0f});

  // ms.add_vertices({{0.0f, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f},
  //                  {0.5f, 0.5, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
  //                  {-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
  //                  {0.8f, 0.0, 1.0f, 0.0f, 1.0f, 0.2f, 1.0f, 0.0f, 0.0f},
  //                  {-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
  //                  {-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
  //                  {-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
  //                  {0.8f, 0.0, 1.0f, 0.0f, 1.0f, 0.2f, 1.0f, 0.0f, 0.0f},
  //                  {-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
  //                  {0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
  //                  {0.8f, 0.0, 1.0f, 0.0f, 1.0f, 0.2f, 1.0f, 0.0f, 0.0f},
  //                  {0.0f, 0.7, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f}});

  // // ms.add_vertex({0.7f, -0.3, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f});
  // ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
  // ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
  // ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
  // ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
  // ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
  // ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
#ifdef __APPLE__
  std::cout << "创建窗口" << std::endl;
  w = new glwindow(800, 600, "test");

  // 主线程必须堵塞,转移main到moved_main中运行
  std::thread t(&moved_main);
  t.detach();

  // 启动渲染循环
  glwindow::start_render();
#endif //__APPLE__
  //    std::cout << "vertex size" << std::to_string(sizeof(vertex)) <<
  //    std::endl; char line[512]; while (true) {
  //      std::cin.getline(line, 512);
  //      std::cout << line << std::endl;
  //    }
  // while (!glfwWindowShouldClose(w)) {
  //   glClearColor(0.23f, 0.23f, 0.23f, 1.0f);
  //   glClear(GL_COLOR_BUFFER_BIT);

  //   ms.bind();
  //   glDrawElements(GL_TRIANGLES, ms._elements.size(), GL_UNSIGNED_INT,
  //                  (void *)0);
  //   // glDrawArrays(GL_TRIANGLES, 0, 3);
  //   ms.unbind();

  //   glfwSwapBuffers(w);
  //   glfwPollEvents();
  // }
}
