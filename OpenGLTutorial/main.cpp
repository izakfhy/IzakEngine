
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GL/glut.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "GraphicsEngine.h"
#include "IZEngine.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


struct CrtBreakAllocSetter {
  CrtBreakAllocSetter() {
    //_crtBreakAlloc = 866;
  }
};

CrtBreakAllocSetter g_crtBreakAllocSetter;
#endif
#endif

#pragma comment(lib,"lib/glfw3")
#pragma comment(lib,"lib/glew32s")

GLuint indices[] = {  // Note that we start from 0!
  0, 1, 3,  // First Triangle
  1, 2, 3   // Second Triangle
};
GLuint EBO;
GLuint mainVBO;
GLuint VAO;
GLuint mainvertexShader;
GLuint shaderProgram;

std::string vertshader("Forward.vert");

void Render(GLFWwindow* window)
{
  while (!glfwWindowShouldClose(window))
  {
    // Check and call events
    glfwPollEvents();

    // Rendering commands here

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Swap the buffers
    glfwSwapBuffers(window);
  }
}


int main()
{

#ifdef _MSC_VER
#ifdef _DEBUG
  _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
  //_CrtSetBreakAlloc(866);
#endif
#endif
  using namespace IZE;
  //int * i = new int(5);
  IZEngine engine;
  engine.Initialize(0,nullptr);
  engine.Run();
  engine.ShutDown();

  return 0;
}