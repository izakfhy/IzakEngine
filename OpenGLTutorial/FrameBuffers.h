
#pragma once

#include "GraphicsEngine.h"
#include "BBEMath.h"


struct FrameBufferbase
{
  FrameBufferbase(void);
  virtual ~FrameBufferbase(void);

  virtual void Init(unsigned int WindowWidth, unsigned int WindowHeight) { glGenBuffers(1, &m_FBO); };
  void Bind(void);
  void Unbind(void);

  GLuint      m_FBO = 0;
  GLuint      m_FBOTexture = 0;
  Vector2     m_Dimensions;
};

struct ShadowMapFBO : public FrameBufferbase
{
public:
  ShadowMapFBO() {}
  ~ShadowMapFBO();

  GLuint m_DepthMap = 0;

  void Init(unsigned int WindowWidth, unsigned int WindowHeight) override final;
};

struct IntermediateFBO : public FrameBufferbase
{
  IntermediateFBO();
  ~IntermediateFBO();

  void Init(unsigned int WindowWidth, unsigned int WindowHeight) override final;

  GLuint m_ColorBuffer = 0;
  GLuint m_MotionBuffer = 0;
  GLuint m_BrightBuffer = 0;
  GLuint m_PickingBuffer = 0;
  GLuint m_DepthBuffer = 0;
};

struct SSAOFBO : public FrameBufferbase
{
  SSAOFBO();
  ~SSAOFBO();

  GLuint m_SSAOGeometryFBO = 0;
  GLuint m_SSAOFBO = 0;
  GLuint m_SSAOBlurFBO = 0;

  GLuint m_PositionTexture = 0;
  GLuint m_NormalTexture = 0;
  GLuint m_DepthTexture = 0;
  GLuint m_SSAOColorBuffer = 0;
  GLuint m_SSAOColorBufferBlur = 0;
  GLuint m_NoiseTexture = 0;

  GLuint m_DepthBuffer = 0;

  //BBE_vector(Vector3) m_SSAOKernal;

  void Init(unsigned int WindowWidth, unsigned int WindowHeight) override final;
};

struct PointLightCubeMap
{
  PointLightCubeMap();
  ~PointLightCubeMap();

  void Init(unsigned int WindowWidth, unsigned int WindowHeight);
  void Bind();
  void Unbind();

  GLuint m_FBO = 0;
  GLuint m_FBOTexture = 0;
  Vector2 m_Dimensions;
};

struct PingPongFBO
{
  PingPongFBO();
  ~PingPongFBO();

  void Init(unsigned int WindowWidth, unsigned int WindowHeight);
  void Bind(int Index);
  void Unbind(void);

  GLuint  m_FBOs[2];
  GLuint  m_FBOTexture[2];
  Vector2 m_Dimensions;
};
