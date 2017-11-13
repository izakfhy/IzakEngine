
#pragma once

#include <algorithm>
#include <cmath>
#include "RefelectionUtils.h"

const int BYTES_IN_MB = 1024 * 1024;
const float PI = 4 * (float)atan(1);
const float TWO_PI = 2 * PI;
const float HALF_PI = PI / 2;
const float RAD_TO_DEG = PI / 180.0f;
const float DEG_TO_RAD = 180.0f / PI;
#define ToRadian(x) (float)(((x) * RAD_TO_DEG))
#define ToDegree(x) (float)(((x) * DEG_TO_RAD))

struct Vector2;
struct Vector3;
struct Vector4;

struct Vector2
{
  union
  {
    struct
    {
      float x, y;
    };
    float m[2];
  };

  Vector2(const float *);
  Vector2(float x = 0, float y = 0);

  // casting for OpenGL
  operator float*                (void);
  operator const float*          (void) const;

  // assignment operators
  Vector2& operator +=           (const Vector2&);
  Vector2& operator -=           (const Vector2&);
  Vector2& operator *=           (float);
  Vector2& operator /=           (float);

  // unary operators
  Vector2 operator -             (void) const;

  // binary operators
  Vector2 operator +             (const Vector2&) const;
  Vector2 operator -             (const Vector2&) const;
  Vector2 operator *             (float) const;
  float operator *               (const Vector2&) const;
  Vector2 operator /             (float) const;

  bool operator ==               (const Vector2&) const;
  bool operator !=               (const Vector2&) const;

  float Length(void) const;
  float LengthSq(void) const;
  float Dot(const Vector2&) const;
  float ScalarTripleProduct(const Vector2&) const;
  void NormalizeThis(void);
  Vector2 Normalize(void) const;

  META_DATA(Vector2);
};

struct Vector3
{
  union
  {
    struct
    {
      float x, y, z;
    };
    struct
    {
      float r, g, b;
    };
    float m[3];
  };

  Vector3(const float *);
  Vector3(const Vector3&);
  Vector3(const Vector4&);
  Vector3(float x = 0, float y = 0, float z = 0);

  // casting for OpenGL
  operator float*                (void);
  operator const float*          (void) const;

  // assignment operators
  Vector3& operator +=           (const Vector3&);
  Vector3& operator -=           (const Vector3&);
  Vector3& operator *=           (float);
  Vector3& operator /=           (float);

  // unary operators
  Vector3 operator -             (void) const;

  // binary operators
  Vector3 operator +             (const Vector3&) const;
  Vector3 operator -             (const Vector3&) const;
  float operator *               (const Vector3&) const;
  Vector3 operator *             (float) const;
  Vector3 operator /             (float) const;

  bool operator ==               (const Vector3&) const;
  bool operator !=               (const Vector3&) const;

  void NormalizeThis(void);
  float Length(void) const;
  float LengthSq(void) const;
  float Dot(const Vector3&) const;
  Vector3 Cross(const Vector3&) const;
  Vector3 Normalize(void) const;

  META_DATA(Vector3);
};

struct Vector4
{
  union
  {
    struct
    {
      float x, y, z, w;
    };
    struct
    {
      float r, g, b, a;
    };
    float m[4];
  };

  Vector4(const float*);
  Vector4(const Vector4&);
  Vector4(const Vector3& xyz, float w);
  Vector4(float x = 0, float y = 0, float z = 0, float w = 0);

  // casting for OpenGL
  operator float*                (void);
  operator const float*          (void) const;

  // assignment operators
  Vector4& operator +=           (const Vector4&);
  Vector4& operator -=           (const Vector4&);
  Vector4& operator *=           (float);
  Vector4& operator /=           (float);

  // unary operators
  Vector4 operator -             (void) const;

  // binary operators
  Vector4 operator +             (const Vector4&) const;
  Vector4 operator -             (const Vector4&) const;
  Vector4 operator *             (float) const;
  Vector4 operator /             (float) const;

  bool operator ==               (const Vector4&) const;
  bool operator !=               (const Vector4&) const;

  void NormalizeThis(void);
  Vector4 Normalize(void) const;
  float Length(void) const;
  float LengthSq(void) const;
  float Dot(const Vector4&) const;

  META_DATA(Vector4);
};

struct Matrix4x4
{
public:
  union
  {
    struct
    {
      float _11, _12, _13, _14;
      float _21, _22, _23, _24;
      float _31, _32, _33, _34;
      float _41, _42, _43, _44;
    };
    float m[4][4];
    float m2[16];
  };

  Matrix4x4(const Matrix4x4&);
  Matrix4x4(const float*);
  Matrix4x4(float _11 = 1, float _12 = 0, float _13 = 0, float _14 = 0,
    float _21 = 0, float _22 = 1, float _23 = 0, float _24 = 0,
    float _31 = 0, float _32 = 0, float _33 = 1, float _34 = 0,
    float _41 = 0, float _42 = 0, float _43 = 0, float _44 = 1);

  // Casting for OpenGL
  operator float*                (void);
  operator const float*          (void) const;

  // assignment operators
  Matrix4x4& operator *=         (const Matrix4x4&);
  Matrix4x4& operator +=         (const Matrix4x4&);
  Matrix4x4& operator -=         (const Matrix4x4&);
  Matrix4x4& operator *=         (float);
  Matrix4x4& operator /=         (float);

  // Inverse matrix              (ORTHO-NORMAL ONLY!)
  Matrix4x4 operator -           (void) const;

  // binary operators
  Vector4 operator *             (const Vector4&) const;
  Matrix4x4 operator *           (const Matrix4x4&) const;
  Matrix4x4 operator +           (const Matrix4x4&) const;
  Matrix4x4 operator -           (const Matrix4x4&) const;
  Matrix4x4 operator *           (float) const;
  Matrix4x4 operator /           (float) const;

  bool operator ==               (const Matrix4x4& rhs) const;
  bool operator !=               (const Matrix4x4& rhs) const;

  void InverseThis(void);
  void ZeroThis(void);
  void MakeRow(int row, const Vector4&);
  void MakeCol(int col, const Vector4&);
  float Determinant(void) const;
  Vector4 Row(int) const;
  Vector4 Col(int) const;
  Matrix4x4 Inverse(void) const;
  Matrix4x4 OrthoNormalInverse(void) const;
  Matrix4x4 MakeIdentity(void);
  Matrix4x4 MakeScaleMatrix(float ScaleX, float ScaleY, float ScaleZ);
  Matrix4x4 MakeRotateMatrix(float RotateX, float RotateY, float RotateZ);
  Matrix4x4 MakeTranslationMatrix(float x, float y, float z);
  Matrix4x4 MakeViewMatrix(const Vector3& Target, const Vector3& Up, const Vector3& CamPos);

  Matrix4x4 MakePerspectiveMatrix(float left, float right, float bottom, float top, float nearVal, float farVal);
  Matrix4x4 MakePerspectiveMatrix(float Aspect, float Near, float Far, float FOV);
  Matrix4x4 MakePerspectiveMatrix(float fov, float width, float height, float near, float far);

  Matrix4x4 MakeOrthoMatrix(float Width, float Height, float Near, float Far);
  Matrix4x4 MakeOrthoMatrix(float left, float right, float bottom, float top, float zNear, float zFar);
  Matrix4x4 MakeLookat(const Vector3& eye, const Vector3& center, const Vector3& up);
  Matrix4x4 Transpose();
  Matrix4x4 Transpose3X3();

  META_DATA(Matrix4x4);
};

struct Quaternion
{
  union
  {
    struct
    {
      float x, y, z, w;
    };
    float m[4];
  };

public:
  Quaternion(float = 0, float = 0, float = 0, float = 1);
  Quaternion(float ang, const Vector3& axis);
  Quaternion(const Vector3& euler);
  Quaternion(const Matrix4x4& rotMatrix);
  Quaternion(const Quaternion& rhs);
  ~Quaternion(void);
  void SetQuat(float ang, const Vector3& axis);
  void SetQuat(const Vector3& euler);
  void SetQuatRad(const Vector3& euler);
  void NormalizeThis(void);
  Vector3 GetEulerAnglesDegrees(void) const;
  Quaternion  Normalize(void);
  Quaternion  GetInverse(void);
  Quaternion  operator *         (const Quaternion& rhs);
  Quaternion& operator *=        (const Quaternion& rhs);
  Vector3     RotateVector(const Vector3& rhs);
  Matrix4x4   GetMatrix(void);
  void Integrate(const Vector3& dv, float dt);
  Quaternion RotateTo(const Vector3& v1, const Vector3& v2);

  META_DATA(Quaternion);
};

const Vector3 RedColor = { 1, 0, 0 };
const Vector3 GreenColor = { 0, 1, 0 };
const Vector3 BlueColor = { 0, 0, 1 };
const Vector3 YellowColor = { 1, 1, 0 };

const Matrix4x4 IDMatrix;


template <typename T>
T lerp(const T& start, const T& end, float percent)
{
  return (start + (end - start) * percent);
}
/*
bool RaytoAABB(const Vector3& CubeCentre, const Vector3& HalfWidths, const Vector3& RayOrigin, const Vector3& Ray, float& t);
bool PointInAABB(const Vector3& Point, const Vector3& CubeCentre, const Vector3& HalfWidths);
bool PointInSphere(const Vector3& Point, const Vector3& SphereCentre, float SphereRadius);*/