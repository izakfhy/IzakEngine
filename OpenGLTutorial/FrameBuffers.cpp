
#include <random> // necessary for generation of random floats (for sample kernel and noise texture)

#include "FrameBuffers.h"
#include "GraphicsEngine.h"


FrameBufferbase::FrameBufferbase()
{
  
  
}

FrameBufferbase::~FrameBufferbase()
{
  if (m_FBO)
  {
    glDeleteBuffers(1, &m_FBO);
    m_FBO = 0;
  }

  if (m_FBOTexture)
  {
    glDeleteTextures(1, &m_FBOTexture);
    m_FBOTexture = 0;
  }
}

void FrameBufferbase::Bind()
{
  glBindBuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
}

void FrameBufferbase::Unbind(void)
{
  glBindBuffer(GL_DRAW_FRAMEBUFFER, 0);
}

ShadowMapFBO::~ShadowMapFBO()
{
  if (m_FBO)
  {
    glDeleteFramebuffers(1, &m_FBO);
    m_FBO = 0;
  }

  if (m_DepthMap)
  {
    glDeleteTextures(1, &m_DepthMap);
    m_DepthMap = 0;
  }
}

void ShadowMapFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
  m_Dimensions = { (float)WindowWidth, (float)WindowHeight };

  // Generate and bind the framebuffer
  glGenFramebuffers(1, &m_FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

  // - Create depth texture
  glGenTextures(1, &m_FBOTexture);
  glBindTexture(GL_TEXTURE_2D, m_FBOTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_FBOTexture, 0);

  // Set the target for the fragment shader outputs
  GLenum drawBufs[] =
  {
    GL_DEPTH_ATTACHMENT,
  };
  glDrawBuffers(sizeof(drawBufs) / sizeof(drawBufs[0]), drawBufs);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
  {
   // MessageBoxA(0, "Failed to generate ShadowMap FBO for Directional Light!", "", MB_OK);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PointLightCubeMap::PointLightCubeMap()
{}

PointLightCubeMap::~PointLightCubeMap()
{
  if (m_FBO)
  {
    glDeleteFramebuffers(1, &m_FBO);
    m_FBO = 0;
  }

  if (m_FBOTexture)
  {
    glDeleteTextures(1, &m_FBOTexture);
    m_FBOTexture = 0;
  }
}

void PointLightCubeMap::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
  m_Dimensions = { (float)WindowWidth, (float)WindowHeight };

  // Configure depth map FBO
  glGenFramebuffers(1, &m_FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

  // Create depth cubemap texture
  glGenTextures(1, &m_FBOTexture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_FBOTexture);

  for (GLuint i = 0; i < 6; ++i)
  {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
      0,
      GL_DEPTH_COMPONENT32,
      WindowWidth,
      WindowHeight,
      0,
      GL_DEPTH_COMPONENT,
      GL_FLOAT,
      NULL);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  // Attach cubemap as depth map FBO's color buffer
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_FBOTexture, 0);

  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    //std::string msg = "PointLight Depth Map FBO not complete! Error: " + std::to_string(status);
    //ERROR_MSG(msg.c_str());
    //STOP_ENGINE;
  }

  //LOG_MSG("PointLightFBO Created!");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointLightCubeMap::Bind()
{
  if (!m_FBO)
  {
    //MessageBoxA(0, "FBO for Point Light not initialized!", "", MB_OK);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void PointLightCubeMap::Unbind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PingPongFBO::PingPongFBO()
{
  for (int i = 0; i < 2; ++i)
  {
    m_FBOs[i] = 0;
    m_FBOTexture[i] = 0;
  }
}

PingPongFBO::~PingPongFBO()
{
  for (int i = 0; i < 2; ++i)
  {
    if (m_FBOs[i])
    {
      glDeleteFramebuffers(1, &m_FBOs[i]);
    }

    if (m_FBOTexture[i])
    {
      glDeleteTextures(1, &m_FBOTexture[i]);
    }
  }
}

void PingPongFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
  m_Dimensions = { (float)WindowWidth, (float)WindowHeight };

  // Generate 2 FBOs and their textures
  glGenFramebuffers(2, m_FBOs);
  glGenTextures(2, m_FBOTexture);

  // color buffers
  for (int i = 0; i < 2; ++i)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[i]);
    glBindTexture(GL_TEXTURE_2D, m_FBOTexture[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FBOTexture[i], 0);
  }

  // Unbind the framebuffer, and revert to default framebuffer
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void PingPongFBO::Bind(int Index)
{
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBOs[Index]);
}

void PingPongFBO::Unbind(void)
{
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

IntermediateFBO::IntermediateFBO()
{}

IntermediateFBO::~IntermediateFBO()
{
  if (m_ColorBuffer)
  {
    glDeleteTextures(1, &m_ColorBuffer);
    m_ColorBuffer = 0;
  }

  if (m_DepthBuffer)
  {
    glDeleteTextures(1, &m_DepthBuffer);
    m_DepthBuffer = 0;
  }

  if (m_MotionBuffer)
  {
    glDeleteTextures(1, &m_MotionBuffer);
    m_MotionBuffer = 0;
  }

  if (m_BrightBuffer)
  {
    glDeleteTextures(1, &m_BrightBuffer);
    m_BrightBuffer = 0;
  }

  if (m_PickingBuffer)
  {
    glDeleteTextures(1, &m_PickingBuffer);
    m_PickingBuffer = 0;
  }
}

void IntermediateFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
  m_Dimensions = { (float)WindowWidth, (float)WindowHeight };

  // Generate and bind the framebuffer
  glGenFramebuffers(1, &m_FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

  // Color buffer
  glGenTextures(1, &m_ColorBuffer);
  glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ColorBuffer, 0);
   glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer, 0);

  // motion buffer
  glGenTextures(1, &m_MotionBuffer);
  glBindTexture(GL_TEXTURE_2D, m_MotionBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_MotionBuffer, 0);

  // bright buffer
  glGenTextures(1, &m_BrightBuffer);
  glBindTexture(GL_TEXTURE_2D, m_BrightBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_BrightBuffer, 0);

  // Create the texture object for the picking buffer
  glGenTextures(1, &m_PickingBuffer);
  glBindTexture(GL_TEXTURE_2D, m_PickingBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, WindowWidth, WindowHeight, 0, GL_RED_INTEGER, GL_INT, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_PickingBuffer, 0);

  // Set the target for the fragment shader outputs
  GLenum drawBufs[] =
  {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
  };

  glDrawBuffers(sizeof(drawBufs) / sizeof(drawBufs[0]), drawBufs);

  // The depth buffer
  glGenRenderbuffers(1, &m_DepthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

  // Verify that the FBO is correct
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Intermediate Framebuffer not complete!" << std::endl;
  //ASSERTMSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Intermediate Framebuffer not complete!");

  // Unbind the framebuffer, and revert to default framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SSAOFBO::SSAOFBO()
{}

SSAOFBO::~SSAOFBO()
{
  if (m_SSAOGeometryFBO)
  {
    glDeleteFramebuffers(1, &m_SSAOGeometryFBO);
    m_SSAOGeometryFBO = 0;
  }

  if (m_SSAOBlurFBO)
  {
    glDeleteFramebuffers(1, &m_SSAOBlurFBO);
    m_SSAOBlurFBO = 0;
  }

  if (m_SSAOFBO)
  {
    glDeleteFramebuffers(1, &m_SSAOFBO);
    m_SSAOFBO = 0;
  }

  if (m_SSAOColorBuffer)
  {
    glDeleteTextures(1, &m_SSAOColorBuffer);
    m_SSAOColorBuffer = 0;
  }

  if (m_SSAOColorBufferBlur)
  {
    glDeleteTextures(1, &m_SSAOColorBufferBlur);
    m_SSAOColorBufferBlur = 0;
  }

  if (m_PositionTexture)
  {
    glDeleteTextures(1, &m_PositionTexture);
    m_PositionTexture = 0;
  }

  if (m_NormalTexture)
  {
    glDeleteTextures(1, &m_NormalTexture);
    m_NormalTexture = 0;
  }

  if (m_DepthTexture)
  {
    glDeleteTextures(1, &m_DepthTexture);
    m_DepthTexture = 0;
  }

  if (m_DepthBuffer)
  {
    glDeleteTextures(1, &m_DepthBuffer);
    m_DepthBuffer = 0;
  }
}

void SSAOFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
  // - SSAO Geometry FBO
  glGenFramebuffers(1, &m_SSAOGeometryFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOGeometryFBO);

  // - Position buffer
  glGenTextures(1, &m_PositionTexture);
  glBindTexture(GL_TEXTURE_2D, m_PositionTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionTexture, 0);
  //ASSERTMSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "SSAO Position Buffer not complete!");

  // - Normal color buffer
  glGenTextures(1, &m_NormalTexture);
  glBindTexture(GL_TEXTURE_2D, m_NormalTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalTexture, 0);
  //ASSERTMSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "SSAO Normal Buffer not complete!");

  // - Depth Texture
  glGenTextures(1, &m_DepthTexture);
  glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_DepthTexture, 0);
  //ASSERTMSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "SSAO Depth Buffer not complete!");

  // Set the target for the fragment shader outputs
  GLenum drawBufs[] =
  {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
  };
  glDrawBuffers(sizeof(drawBufs) / sizeof(drawBufs[0]), drawBufs);

  // The depth buffer
  glGenRenderbuffers(1, &m_DepthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);

  // - SSAO color buffer
  glGenFramebuffers(1, &m_SSAOFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOFBO);
  glGenTextures(1, &m_SSAOColorBuffer);
  glBindTexture(GL_TEXTURE_2D, m_SSAOColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSAOColorBuffer, 0);
  //ASSERTMSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "SSAO Color Buffer not complete!");

  // - and blur stage
  glGenFramebuffers(1, &m_SSAOBlurFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOBlurFBO);
  glGenTextures(1, &m_SSAOColorBufferBlur);
  glBindTexture(GL_TEXTURE_2D, m_SSAOColorBufferBlur);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SSAOColorBufferBlur, 0);
  //ASSERTMSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "SSAO Color Blur Buffer not complete!");

  // Set the target for the fragment shader outputs
  GLenum drawBufs2[] =
  {
    GL_COLOR_ATTACHMENT0,
  };
  glDrawBuffers(sizeof(drawBufs2) / sizeof(drawBufs2[0]), drawBufs2);

  // Verify that the FBO is correct
 // ASSERTMSG(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "SSAO Framebuffer not complete!");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Sample kernel, generates random floats between 0.0 and 1.0
  std::uniform_real_distribution<GLfloat> randomFloats(0, 1);
  std::default_random_engine generator;
  const int KERNAL_SAMPLES = 64;
  for (GLuint i = 0; i < KERNAL_SAMPLES; ++i)
  {
    Vector3 sample(randomFloats(generator) * 2 - 1, randomFloats(generator) * 2 - 1, randomFloats(generator));
    sample.NormalizeThis();
    sample *= randomFloats(generator);
    GLfloat scale = GLfloat(i) / KERNAL_SAMPLES;

    // Scale samples s.t. they're more aligned to center of kernel
    scale = lerp(0.1f, 1.0f, scale * scale);
    sample *= scale;
    //m_SSAOKernal.push_back(sample);
  }

  // Noise texture
  std::vector<Vector3> SSAONoise;
  const int NOISE_TEX_SIZE = 16;
  for (GLuint i = 0; i < NOISE_TEX_SIZE; i++)
  {
    // rotate around z-axis (in tangent space)
    Vector3 noise(randomFloats(generator) * 2 - 1, randomFloats(generator) * 2 - 1, 0);
    SSAONoise.push_back(noise);
  }
  glGenTextures(1, &m_NoiseTexture);
  glBindTexture(GL_TEXTURE_2D, m_NoiseTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &SSAONoise[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
