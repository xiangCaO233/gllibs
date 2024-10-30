#include "gls/mesh.h"
#include "gls/shader.h"
#include <iostream>

int main() {
  shader s("../assets/shader/vertex.glsl", "../assets/shader/fragment.glsl");
  std::cout << "编译着色器完成" << std::endl;
  s.use();
  mesh ms(GL_DYNAMIC_DRAW);
  ms.add_vertex({-0.5f, -0.5, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f});
  ms.add_vertex({0.5f, -0.5, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f});
  ms.add_vertex({0.0f, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f});
  ms.add_vertex({0.5f, 0.5, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f});
  ms.add_vertex({-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f});
  ms.add_vertex({0.8f, 0.0, 1.0f, 0.0f, 1.0f, 0.2f, 1.0f, 0.0f, 0.0f});
  ms.add_vertex({0.0f, 0.7, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f});
  ms.add_vertex({0.2f, -0.3, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f});
  ms.add_vertex({-0.8f, -0.5, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f});
  ms.add_vertex({0.2f, 0.5, 1.0f, 1.0f, 1.0f, 0.1f, 1.0f, 0.0f, 0.0f});

  ms.add_vertices({{0.0f, 0.5, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f},
                   {0.5f, 0.5, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f},
                   {-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
                   {0.8f, 0.0, 1.0f, 0.0f, 1.0f, 0.2f, 1.0f, 0.0f, 0.0f},
                   {-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
                   {-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
                   {-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
                   {0.8f, 0.0, 1.0f, 0.0f, 1.0f, 0.2f, 1.0f, 0.0f, 0.0f},
                   {-0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
                   {0.5f, 0.5, 1.0f, 0.3f, 0.2f, 1.0f, 1.0f, 0.0f, 0.0f},
                   {0.8f, 0.0, 1.0f, 0.0f, 1.0f, 0.2f, 1.0f, 0.0f, 0.0f},
                   {0.0f, 0.7, 1.0f, 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f}});

  // ms.add_vertex({0.7f, -0.3, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f});
  ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
  ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
  ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
  ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
  ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
  ms.add_elements({0, 1, 2, 3, 6, 4, 5, 1, 3});
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
