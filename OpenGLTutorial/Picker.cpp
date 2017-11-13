#include "Picker.h"
#include "GameObject.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GL/glut.h"
#include "Globals.h"
#include "IZEngine.h"
#include "AllComponents.h"
#include "SystemComponentHeaders.h"

Picker* Picker::m_instance = nullptr;

Picker::Picker() :m_curObj(nullptr)
{
  m_instance = this;
}
Picker::~Picker()
{
}

Picker* Picker::GetInstance()
{
  return m_instance;
}

GameObject* Picker::GetPickedObj()
{
  return m_curObj;
}
void Picker::ReadPixelPicking(unsigned FrameBuffer)
{
  // bind GBuffer's Picking texture
  glBindFramebuffer(GL_READ_FRAMEBUFFER, FrameBuffer);

  glReadBuffer(GL_COLOR_ATTACHMENT3);
  //std::cout << gluErrorString(glGetError()) << std::endl;
  int id = -1;
  int inputx = (int)g_input->m_CursorViewPortPos.x;
  int enginey = (int)g_engine->m_WindowResolution.y;
  int inputy = (int)g_input->m_CursorViewPortPos.y;
  glReadPixels((int)g_input->m_CursorViewPortPos.x,
    (int)(g_engine->m_WindowResolution.y - g_input->m_CursorViewPortPos.y),
    1,
    1,
    GL_RED_INTEGER,
    GL_INT,
    &id);

  std::cout << "ID: " << id << std::endl;

  // reset buffer
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}