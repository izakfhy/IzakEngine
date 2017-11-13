
#include "BBEMath.h"

DEFINE_META(Vector2)
{
  ADD_MEMBER(x, true);
  ADD_MEMBER(y, true);

}


DEFINE_META(Vector3)
{
  ADD_MEMBER(x, true);
  ADD_MEMBER(y, true);
  ADD_MEMBER(z, true );

}

DEFINE_META(Vector4)
{
  ADD_MEMBER(x, true);
  ADD_MEMBER(y, true);
  ADD_MEMBER(z, true);
  ADD_MEMBER(w, true);

}

DEFINE_META(Matrix4x4)
{

  ADD_MEMBER(_11,false);
  ADD_MEMBER(_12,false);
  ADD_MEMBER(_13,false);
  ADD_MEMBER(_14,false);
  ADD_MEMBER(_21,false);
  ADD_MEMBER(_22,false);
  ADD_MEMBER(_23,false);
  ADD_MEMBER(_24,false);
  ADD_MEMBER(_31,false);
  ADD_MEMBER(_32,false);
  ADD_MEMBER(_33,false);
  ADD_MEMBER(_34,false);
  ADD_MEMBER(_41,false);
  ADD_MEMBER(_42,false);
  ADD_MEMBER(_43,false);
  ADD_MEMBER(_44,false);
}

DEFINE_META(Quaternion)
{
  ADD_MEMBER(x, true);
  ADD_MEMBER(y, true);
  ADD_MEMBER(z, true);
  ADD_MEMBER(w, true);
  ADD_MEMBER(m, true);
}

Vector2::Vector2(const float * f)
  :
  x(f[0]), y(f[1])
{}

Vector2::Vector2(float x_, float y_)
  :
  x(x_), y(y_)
{}

Vector2::operator float* ()
{
  return (float*)&x;
}

Vector2::operator const float* () const
{
  return (const float*)&x;
}

Vector2& Vector2::operator += (const Vector2& v)
{
  x += v.x;
  y += v.y;
  return *this;
}

Vector2& Vector2::operator -= (const Vector2& v)
{
  x -= v.x;
  y -= v.y;
  return *this;
}

Vector2& Vector2::operator *= (float f)
{
  x *= f;
  y *= f;
  return *this;
}

Vector2& Vector2::operator /= (float f)
{
  const float fInv = 1.0f / f;
  x *= fInv;
  y *= fInv;
  return *this;
}

Vector2 Vector2::operator - () const
{
  return Vector2(-x, -y);
}

Vector2 Vector2::operator + (const Vector2& v) const
{
  return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator - (const Vector2& v) const
{
  return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator * (float f) const
{
  return Vector2(x * f, y * f);
}

float Vector2::operator * (const Vector2& rhs) const
{
  return (x * rhs.x + y * rhs.y);
}

Vector2 Vector2::operator / (float f) const
{
  const float fInv = 1.0f / f;
  return Vector2(x * fInv, y * fInv);
}

bool Vector2::operator == (const Vector2& v) const
{
  return x == v.x && y == v.y;
}

bool Vector2::operator != (const Vector2& v) const
{
  return x != v.x || y != v.y;
}

void Vector2::NormalizeThis()
{
  const float l = Length();

  x /= l;
  y /= l;
}

Vector2 Vector2::Normalize() const
{
  const float l = Length();
  Vector2 v(*this);
  v.x /= l;
  v.y /= l;

  return v;
}

float Vector2::Length() const
{
  return std::sqrt(LengthSq());
}

float Vector2::LengthSq() const
{
  return Dot(*this);
}

float Vector2::Dot(const Vector2& rhs) const
{
  return x * rhs.x + y * rhs.y;
}

float Vector2::ScalarTripleProduct(const Vector2& rhs) const
{
  return x * rhs.y - y * rhs.x;
}

Vector3::Vector3(const float * f)
  :
  x(f[0]), y(f[1]), z(f[3])
{}

Vector3::Vector3(const Vector3& v)
  :
  x(v.x), y(v.y), z(v.z)
{}

Vector3::Vector3(const Vector4& v)
  :
  x(v.x), y(v.y), z(v.z)
{}

Vector3::Vector3(float x_, float y_, float z_)
  :
  x(x_), y(y_), z(z_)
{}

Vector3::operator float* ()
{
  return (float*)&x;
}

Vector3::operator const float* () const
{
  return (const float*)&x;
}

Vector3& Vector3::operator += (const Vector3& v)
{
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

Vector3& Vector3::operator -= (const Vector3& v)
{
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

Vector3& Vector3::operator *= (float f)
{
  x *= f;
  y *= f;
  z *= f;
  return *this;
}

Vector3& Vector3::operator /= (float f)
{
  const float fInv = 1.0f / f;
  x *= fInv;
  y *= fInv;
  z *= fInv;
  return *this;
}

Vector3 Vector3::operator - () const
{
  return Vector3(-x, -y, -z);
}

Vector3 Vector3::operator + (const Vector3& v) const
{
  return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::operator - (const Vector3& v) const
{
  return Vector3(x - v.x, y - v.y, z - v.z);
}

Vector3 Vector3::operator * (float f) const
{
  return Vector3(x * f, y * f, z * f);
}

Vector3 Vector3::operator / (float f) const
{
  const float fInv = 1.0f / f;
  return Vector3(x * fInv, y * fInv, z * fInv);
}

float Vector3::operator * (const Vector3& v) const
{
  return Dot(v);
}

bool Vector3::operator == (const Vector3& v) const
{
  return x == v.x && y == v.y && z == v.z;
}

bool Vector3::operator != (const Vector3& v) const
{
  return x != v.x || y != v.y || z != v.z;
}

Vector3 Vector3::Normalize() const
{
  const float l = Length();
  if (l < FLT_EPSILON)
  {
    return *this;
  }
  Vector3 v(*this);
  v.x /= l;
  v.y /= l;
  v.z /= l;
  return v;
}

void Vector3::NormalizeThis()
{
  const float l = Length();
  if (l < FLT_EPSILON)
  {
    return;
  }
  x /= l;
  y /= l;
  z /= l;
}

float Vector3::Length() const
{
  return std::sqrt(LengthSq());
}

float Vector3::LengthSq() const
{
  return Dot(*this);
}

float Vector3::Dot(const Vector3& rhs) const
{
  return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}

Vector3 Vector3::Cross(const Vector3& rhs) const
{
  return{ ((y * rhs.z) - (rhs.y * z)), ((rhs.x * z) - (rhs.z * x)), ((x * rhs.y) - (rhs.x * y)) };
}

Vector4::Vector4(const float* pf)
  :
  x(pf[0]), y(pf[1]), z(pf[2]), w(pf[3])
{}

Vector4::Vector4(const Vector4& v)
  :
  x(v.x), y(v.y), z(v.z), w(v.w)
{}

Vector4::Vector4(const Vector3& v, float f)
  :
  x(v.x), y(v.y), z(v.z), w(f)
{}

Vector4::Vector4(float fx, float fy, float fz, float fw)
  :
  x(fx), y(fy), z(fz), w(fw)
{}

Vector4::operator float* ()
{
  return (float*)&x;
}

Vector4::operator const float* () const
{
  return (const float*)&x;
}

Vector4& Vector4::operator += (const Vector4& v)
{
  x += v.x;
  y += v.y;
  z += v.z;
  w += v.w;
  return *this;
}

Vector4& Vector4::operator -= (const Vector4& v)
{
  x -= v.x;
  y -= v.y;
  z -= v.z;
  w -= v.w;
  return *this;
}

Vector4& Vector4::operator *= (float f)
{
  x *= f;
  y *= f;
  z *= f;
  w *= f;
  return *this;
}

Vector4& Vector4::operator /= (float f)
{
  const float fInv = 1.0f / f;
  x *= fInv;
  y *= fInv;
  z *= fInv;
  w *= fInv;
  return *this;
}

Vector4 Vector4::operator - () const
{
  return Vector4(-x, -y, -z, -w);
}

Vector4 Vector4::operator + (const Vector4& v) const
{
  return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator - (const Vector4& v) const
{
  return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator * (float f) const
{
  return Vector4(x * f, y * f, z * f, w * f);
}

Vector4 Vector4::operator / (float f) const
{
  const float fInv = 1.0f / f;
  return Vector4(x * fInv, y * fInv, z * fInv, w * fInv);
}

bool Vector4::operator == (const Vector4& v) const
{
  return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vector4::operator != (const Vector4& v) const
{
  return x != v.x || y != v.y || z != v.z || w != v.w;
}

Vector4 Vector4::Normalize() const
{
  Vector4 v(*this);
  const float l = 1 / Length();
  v.x *= l;
  v.y *= l;
  v.z *= l;
  v.w *= l;
  return v;
}

void Vector4::NormalizeThis()
{
  const float l = 1 / Length();
  x *= l;
  y *= l;
  z *= l;
  w *= l;
}

float Vector4::Length() const
{
  return std::sqrt(LengthSq());
}

float Vector4::LengthSq() const
{
  return Dot(*this);
}

float Vector4::Dot(const Vector4& rhs) const
{
  return ((x * rhs.x) + (y * rhs.y)) + ((z * rhs.z) + (w * rhs.w));
}

Matrix4x4::Matrix4x4(const Matrix4x4& rhs)
{
  memcpy(&_11, &rhs, sizeof(Matrix4x4));
}

Matrix4x4::Matrix4x4(const float* pf)
{
  memcpy(&_11, pf, sizeof(Matrix4x4));
}

Matrix4x4::Matrix4x4(float _11, float _12, float _13, float _14,
  float _21, float _22, float _23, float _24,
  float _31, float _32, float _33, float _34,
  float _41, float _42, float _43, float _44)
  :
  _11(_11), _12(_12), _13(_13), _14(_14),
  _21(_21), _22(_22), _23(_23), _24(_24),
  _31(_31), _32(_32), _33(_33), _34(_34),
  _41(_41), _42(_42), _43(_43), _44(_44)
{}

Matrix4x4::operator float*()
{
  return (float*)&_11;
}

Matrix4x4::operator const float*() const
{
  return (const float*)&_11;
}

Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& rhs)
{
  Matrix4x4 Ret;

  for (unsigned int i = 0; i < 4; i++)
  {
    for (unsigned int j = 0; j < 4; j++)
    {
      Ret.m[i][j] = m[i][0] * rhs.m[0][j] +
        m[i][1] * rhs.m[1][j] +
        m[i][2] * rhs.m[2][j] +
        m[i][3] * rhs.m[3][j];
    }
  }

  *this = Ret;
  return *this;
}

Matrix4x4& Matrix4x4::operator += (const Matrix4x4& mat)
{
  _11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
  _21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
  _31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
  _41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
  return *this;
}

Matrix4x4& Matrix4x4::operator -= (const Matrix4x4& mat)
{
  _11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
  _21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
  _31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
  _41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
  return *this;
}

Matrix4x4& Matrix4x4::operator *= (float f)
{
  _11 *= f; _12 *= f; _13 *= f; _14 *= f;
  _21 *= f; _22 *= f; _23 *= f; _24 *= f;
  _31 *= f; _32 *= f; _33 *= f; _34 *= f;
  _41 *= f; _42 *= f; _43 *= f; _44 *= f;
  return *this;
}

Matrix4x4& Matrix4x4::operator /= (float f)
{
  const float fInv = 1.0f / f;
  _11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
  _21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
  _31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
  _41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
  return *this;
}

Vector4 Matrix4x4::operator * (const Vector4& v) const
{
  Vector4 ret;
  ret.x = (_11 * v.x) + (_12 * v.y) + (_13 * v.z) + (_14 * v.w);
  ret.y = (_21 * v.x) + (_22 * v.y) + (_23 * v.z) + (_24 * v.w);
  ret.z = (_31 * v.x) + (_32 * v.y) + (_33 * v.z) + (_34 * v.w);
  ret.w = (_41 * v.x) + (_42 * v.y) + (_43 * v.z) + (_44 * v.w);
  return ret;
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& rhs) const
{
  Matrix4x4 ret(*this);
  ret *= rhs;
  return ret;
}

Matrix4x4 Matrix4x4::operator + (const Matrix4x4& mat) const
{
  return Matrix4x4(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
    _21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
    _31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
    _41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
}

Matrix4x4 Matrix4x4::operator - () const
{
  Matrix4x4 Result(*this);
  Result.InverseThis();
  return Result;
}

Matrix4x4 Matrix4x4::operator - (const Matrix4x4& mat) const
{
  return Matrix4x4(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
    _21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
    _31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
    _41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
}

Matrix4x4 Matrix4x4::operator * (float f) const
{
  return Matrix4x4(_11 * f, _12 * f, _13 * f, _14 * f,
    _21 * f, _22 * f, _23 * f, _24 * f,
    _31 * f, _32 * f, _33 * f, _34 * f,
    _41 * f, _42 * f, _43 * f, _44 * f);
}

Matrix4x4 Matrix4x4::operator / (float f) const
{
  const float fInv = 1.0f / f;
  return Matrix4x4(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
    _21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
    _31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
    _41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);
}

bool Matrix4x4::operator == (const Matrix4x4& rhs) const
{
  return 0 == memcmp(this, &rhs, sizeof(Matrix4x4));
}

bool Matrix4x4::operator != (const Matrix4x4& rhs) const
{
  return 0 != memcmp(this, &rhs, sizeof(Matrix4x4));
}

Vector4 Matrix4x4::Row(int r) const
{
  return Vector4(m[r][0], m[r][1], m[r][2], m[r][3]);
}

Vector4 Matrix4x4::Col(int c) const
{
  return Vector4(m[0][c], m[1][c], m[2][c], m[3][c]);
}

float Matrix4x4::Determinant() const
{
  return m[0][0] * m[1][1] * m[2][2] * m[3][3] - m[0][0] * m[1][1] * m[2][3] * m[3][2] + m[0][0] * m[1][2] * m[2][3] * m[3][1] - m[0][0] * m[1][2] * m[2][1] * m[3][3]
    + m[0][0] * m[1][3] * m[2][1] * m[3][2] - m[0][0] * m[1][3] * m[2][2] * m[3][1] - m[0][1] * m[1][2] * m[2][3] * m[3][0] + m[0][1] * m[1][2] * m[2][0] * m[3][3]
    - m[0][1] * m[1][3] * m[2][0] * m[3][2] + m[0][1] * m[1][3] * m[2][2] * m[3][0] - m[0][1] * m[1][0] * m[2][2] * m[3][3] + m[0][1] * m[1][0] * m[2][3] * m[3][2]
    + m[0][2] * m[1][3] * m[2][0] * m[3][1] - m[0][2] * m[1][3] * m[2][1] * m[3][0] + m[0][2] * m[1][0] * m[2][1] * m[3][3] - m[0][2] * m[1][0] * m[2][3] * m[3][1]
    + m[0][2] * m[1][1] * m[2][3] * m[3][0] - m[0][2] * m[1][1] * m[2][0] * m[3][3] - m[0][3] * m[1][0] * m[2][1] * m[3][2] + m[0][3] * m[1][0] * m[2][2] * m[3][1]
    - m[0][3] * m[1][1] * m[2][2] * m[3][0] + m[0][3] * m[1][1] * m[2][0] * m[3][2] - m[0][3] * m[1][2] * m[2][0] * m[3][1] + m[0][3] * m[1][2] * m[2][1] * m[3][0];
}

Matrix4x4 Matrix4x4::Inverse() const
{
  // Compute the reciprocal determinant
  const float det = Determinant();
  if (det == 0.0f)
  {
    return *this;
  }

  const float invdet = 1.0f / det;

  Matrix4x4 res;
  res.m[0][0] = invdet  * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
  res.m[0][1] = -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[0][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
  res.m[0][2] = invdet  * (m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][1] - m[1][1] * m[3][3]) + m[0][3] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]));
  res.m[0][3] = -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][1] - m[1][1] * m[2][3]) + m[0][3] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
  res.m[1][0] = -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
  res.m[1][1] = invdet  * (m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
  res.m[1][2] = -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][2] - m[1][2] * m[3][0]));
  res.m[1][3] = invdet  * (m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]));
  res.m[2][0] = invdet  * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[1][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res.m[2][1] = -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[0][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res.m[2][2] = invdet  * (m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) + m[0][1] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
  res.m[2][3] = -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) + m[0][1] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
  res.m[3][0] = -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[1][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res.m[3][1] = invdet  * (m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[0][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[0][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
  res.m[3][2] = -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) + m[0][1] * (m[1][2] * m[3][0] - m[1][0] * m[3][2]) + m[0][2] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
  res.m[3][3] = invdet  * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));

  return res;
}

Matrix4x4 Matrix4x4::MakeIdentity()
{
  m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
  m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
  m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
  m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
  return *this;
}

Matrix4x4 Matrix4x4::OrthoNormalInverse() const
{
  Matrix4x4 m;

  m._11 = _11;
  m._12 = _21;
  m._13 = _31;
  m._14 = -(Col(3).Dot(Col(0)));

  m._21 = _12;
  m._22 = _22;
  m._23 = _32;
  m._24 = -(Col(3).Dot(Col(1)));

  m._31 = _13;
  m._32 = _23;
  m._33 = _33;
  m._34 = -(Col(3).Dot(Col(2)));

  m._41 = 0;
  m._42 = 0;
  m._43 = 0;
  m._44 = 1;

  return m;
}

void Matrix4x4::MakeRow(int r, const Vector4& v)
{
  m[r][0] = v.x;
  m[r][1] = v.y;
  m[r][2] = v.z;
  m[r][3] = v.w;
}

void Matrix4x4::MakeCol(int c, const Vector4& v)
{
  m[0][c] = v.x;
  m[1][c] = v.y;
  m[2][c] = v.z;
  m[3][c] = v.w;
}

void Matrix4x4::InverseThis()
{
  *this = Inverse();
}

void Matrix4x4::ZeroThis()
{
  for (int i = 0; i < 16; ++i)
    m2[i] = 0;
}

Matrix4x4 Matrix4x4::MakeScaleMatrix(float ScaleX, float ScaleY, float ScaleZ)
{
  m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
  m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
  m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
  m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;

  return *this;
}

Matrix4x4 Matrix4x4::MakeRotateMatrix(float RotateX, float RotateY, float RotateZ)
{
  Matrix4x4 rx, ry, rz;

  const float x = ToRadian(RotateX);
  const float y = ToRadian(RotateY);
  const float z = ToRadian(RotateZ);

  rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f;    rx.m[0][2] = 0.0f;     rx.m[0][3] = 0.0f;
  rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
  rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x);  rx.m[2][3] = 0.0f;
  rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f;    rx.m[3][2] = 0.0f;     rx.m[3][3] = 1.0f;

  ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
  ry.m[1][0] = 0.0f;    ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f;     ry.m[1][3] = 0.0f;
  ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y);  ry.m[2][3] = 0.0f;
  ry.m[3][0] = 0.0f;    ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f;     ry.m[3][3] = 1.0f;

  rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
  rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z);  rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
  rz.m[2][0] = 0.0f;    rz.m[2][1] = 0.0f;     rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
  rz.m[3][0] = 0.0f;    rz.m[3][1] = 0.0f;     rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

  *this = rz * ry * rx;
  return *this;
}

Matrix4x4 Matrix4x4::MakeTranslationMatrix(float x, float y, float z)
{
  m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
  m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
  m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
  m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;

  return *this;
}

Matrix4x4 Matrix4x4::MakeViewMatrix(const Vector3& Target, const Vector3& Up, const Vector3& CamPos)
{
  const Vector3 W = (CamPos - Target).Normalize();
  const Vector3 U = (Up.Cross(W)).Normalize();
  const Vector3 V = W.Cross(U);

  m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = -(U.Dot(CamPos));
  m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = -(V.Dot(CamPos));
  m[2][0] = W.x;   m[2][1] = W.y;   m[2][2] = W.z;   m[2][3] = -(W.Dot(CamPos));
  m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;

  return *this;
}

Matrix4x4 Matrix4x4::MakeLookat(const Vector3 & eye, const Vector3 & center, const Vector3 & up)
{
  MakeIdentity();

  const Vector3 f = (center - eye).Normalize();
  const Vector3 s = (f.Cross(up)).Normalize();
  const Vector3 u = s.Cross(f);

  m[0][0] = s.x;
  m[0][1] = s.y;
  m[0][2] = s.z;
  m[1][0] = u.x;
  m[1][1] = u.y;
  m[1][2] = u.z;
  m[2][0] = -f.x;
  m[2][1] = -f.y;
  m[2][2] = -f.z;
  m[0][3] = -s.Dot(eye);// -dot(s, eye);
  m[1][3] = -u.Dot(eye);// -dot(u, eye);
  m[2][3] = f.Dot(eye); // dot(f, eye);

  return *this;
}
Matrix4x4 Matrix4x4::Transpose3X3()
{
  Matrix4x4 temp;
  temp.MakeIdentity();
  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      float tmp = temp.m[i][j];
      temp.m[i][j] = temp.m[j][i];
      temp.m[j][i] = tmp;
    }
  }
  return temp;
}

Matrix4x4 Matrix4x4::Transpose()
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      float tmp = m[i][j];
      m[i][j] = m[j][i];
      m[j][i] = tmp;
    }
  }
  return *this;
}

Matrix4x4 Matrix4x4::MakePerspectiveMatrix(float left, float right, float bottom, float top, float nearVal, float farVal)
{
  Matrix4x4 Result;
  for (int i = 0; i < 16; ++i)
  {
    Result.m2[i] = 0;
  }
  Result.m[0][0] = (2 * nearVal) / (right - left);
  Result.m[1][1] = (2 * nearVal) / (top - bottom);
  Result.m[0][2] = (right + left) / (right - left);
  Result.m[1][2] = (top + bottom) / (top - bottom);
  Result.m[2][2] = -(farVal + nearVal) / (farVal - nearVal);
  Result.m[3][2] = -1;
  Result.m[2][3] = -(2 * farVal * nearVal) / (farVal - nearVal);
  *this = Result;
  return Result;
}

Matrix4x4 Matrix4x4::MakePerspectiveMatrix(float Aspect, float Near, float Far, float FOV)
{
  const float zRange = Near - Far;
  const float tanHalfFOV = tanf(FOV * 0.5f);

  m[0][0] = 1.0f / (tanHalfFOV * Aspect);
  m[0][1] = 0.0f;
  m[0][2] = 0.0f;
  m[0][3] = 0.0;

  m[1][0] = 0.0f;
  m[1][1] = 1.0f / tanHalfFOV;
  m[1][2] = 0.0f;
  m[1][3] = 0.0;

  m[2][0] = 0.0f;
  m[2][1] = 0.0f;
  m[2][2] = (Near + Far) / zRange;
  m[2][3] = 2.0f*Far*Near / zRange;

  m[3][0] = 0.0f;
  m[3][1] = 0.0f;
  m[3][2] = -1.0f;
  m[3][3] = 0.0;

  return *this;
}

Matrix4x4 Matrix4x4::MakePerspectiveMatrix(float fov, float width, float height, float near, float far)
{
  float rad = fov;
  float h = cos(0.5f * rad) / sin(0.5f * rad);
  float w = h * height / width;

  Matrix4x4 Result;
  Result.ZeroThis();
  Result.m[0][0] = w;
  Result.m[1][1] = h;
  Result.m[2][2] = -(far + near) / (far - near);
  Result.m[3][2] = -1;
  Result.m[2][3] = -(2 * far * near) / (far - near);
  return Result;
}

Matrix4x4 Matrix4x4::MakeOrthoMatrix(float w, float h, float zn, float zf)
{
  MakeIdentity();
  const float depth = zf - zn;
  m[0][0] = 2 / w;
  m[1][1] = 2 / h;
  m[2][2] = -2 / depth;
  m[2][3] = -(zn + zf) / depth;

  return *this;
}

Matrix4x4 Matrix4x4::MakeOrthoMatrix(float left, float right, float bottom, float top, float zNear, float zFar)
{
  Matrix4x4 Result;
  Result.m[0][0] = 2.0f / (right - left);
  Result.m[1][1] = 2.0f / (top - bottom);
  Result.m[2][2] = -2.0f / (zFar - zNear);
  Result.m[0][3] = -(right + left) / (right - left);
  Result.m[1][3] = -(top + bottom) / (top - bottom);
  Result.m[2][3] = -(zFar + zNear) / (zFar - zNear);
  *this = Result;
  return *this;
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
  : x(_x), y(_y), z(_z), w(_w)
{}

Quaternion::Quaternion(float ang, const Vector3& axis)
{
  SetQuat(ang, axis);
}

Quaternion::Quaternion(const Vector3 & euler)
{
  SetQuat(euler);
}

Quaternion::Quaternion(const Matrix4x4 & rotMatrix)
{
  w = sqrtf(1 + rotMatrix.m[0][0] + rotMatrix.m[1][1] + rotMatrix.m[2][2]) / 2.f;
  x = (rotMatrix.m[2][1] - rotMatrix.m[1][2]) / (4.f * w);
  y = (rotMatrix.m[0][2] - rotMatrix.m[2][0]) / (4.f * w);
  z = (rotMatrix.m[1][0] - rotMatrix.m[0][1]) / (4.f * w);
}

Quaternion::Quaternion(const Quaternion & rhs)
  : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
{}

Quaternion::~Quaternion()
{}

void Quaternion::SetQuat(float ang, const Vector3 & axis)
{
  ang = ToRadian(ang);
  w = cosf(ang * 0.5f);
  ang = sinf(ang * 0.5f);
  x = ang * axis.x;
  y = ang * axis.y;
  z = ang * axis.z;
}

void Quaternion::SetQuat(const Vector3 & euler)
{
  Vector3 radVec(ToRadian(euler.x), ToRadian(euler.y), ToRadian(euler.z));

  float cosY = cosf(radVec.y * 0.5f);//c1
  float cosX = cosf(radVec.x * 0.5f);//c2
  float cosZ = cosf(radVec.z * 0.5f);//c3

  float sinY = sinf(radVec.y * 0.5f);//s1
  float sinX = sinf(radVec.x * 0.5f);//s2
  float sinZ = sinf(radVec.z * 0.5f);//s3

  w = (cosY * cosX * cosZ) - (sinY * sinX * sinZ);
  x = (sinY * sinX * cosZ) + (cosY * cosX * sinZ);
  y = (sinY * cosX * cosZ) + (cosY * sinX * sinZ);
  z = (cosY * sinX * cosZ) - (sinY * cosX * sinZ);
}

void Quaternion::SetQuatRad(const Vector3 & euler)
{
  float cosY = cosf(euler.y * 0.5f);//c1
  float cosX = cosf(euler.x * 0.5f);//c2
  float cosZ = cosf(euler.z * 0.5f);//c3

  float sinY = sinf(euler.y * 0.5f);//s1
  float sinX = sinf(euler.x * 0.5f);//s2
  float sinZ = sinf(euler.z * 0.5f);//s3

  w = (cosY * cosX * cosZ) - (sinY * sinX * sinZ);
  x = (sinY * sinX * cosZ) + (cosY * cosX * sinZ);
  y = (sinY * cosX * cosZ) + (cosY * sinX * sinZ);
  z = (cosY * sinX * cosZ) - (sinY * cosX * sinZ);
}

void Quaternion::NormalizeThis()
{
  *this = Normalize();
}

Vector3 Quaternion::GetEulerAnglesDegrees() const
{
  Vector3 v;
  v.x = ToDegree(asin(2.f * x * y + 2.f * z * w));
  v.y = ToDegree(atan2(2.f * y * w - 2.f * x * z, 1.f - 2.f * y *y - 2.f * z * z));
  v.z = ToDegree(atan2(2.f * x * w - 2 * y * z, 1.f - 2.f * x * x - 2.f * z * z));
  return v;
}

Quaternion Quaternion::Normalize()
{
  float LenSq = w*w + x*x + y*y + z*z;

  if (abs(LenSq - 1) < 0.001f)return *this;

  float magnitude = sqrt(LenSq);

  return Quaternion(x / magnitude, y / magnitude, z / magnitude, w / magnitude);
}

Quaternion Quaternion::GetInverse()
{
  return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::operator*(const Quaternion & rhs)
{
  Quaternion ret(*this);
  return ret *= rhs;
}

Quaternion & Quaternion::operator*=(const Quaternion & rhs)
{
  Quaternion mul;
  mul.w = (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z);
  mul.x = (w * rhs.x) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y);
  mul.y = (w * rhs.y) - (x * rhs.z) + (y * rhs.w) + (z * rhs.x);
  mul.z = (w * rhs.z) + (x * rhs.y) - (y * rhs.x) + (z * rhs.w);
  *this = mul;
  return *this;
}

Vector3 Quaternion::RotateVector(const Vector3 & rhs)
{
  Quaternion ret(rhs.x, rhs.y, rhs.z, 0.f);
  ret = *this * ret * GetInverse();
  return Vector3(ret.x, ret.y, ret.z);
}

Matrix4x4 Quaternion::GetMatrix()
{
  float sqX = x * x;
  float sqY = y * y;
  float sqZ = z * z;
  Matrix4x4 ret
    (
    1.f - (2.f * sqY) - (2.f * sqZ), (2.f*x * y) - (2.f * z * w), (2.f * x * z) + (2.f * y * w), 0.f,
    (2.f * x * y) + (2.f * z * w), 1.f - (2.f * sqX) - (2.f * sqZ), (2.f * y * z) - (2.f * x * w), 0.f,
    (2.f * x * z) - (2.f * y * w), (2.f * y * z) + (2.f * x * w), 1.f - (2.f * sqX) - (2.f * sqY), 0.f,
    0.f, 0.f, 0.f, 1.f
    );
  return ret;
}


void Quaternion::Integrate(const Vector3& dv, float dt)
{

  Quaternion q(dv.x * dt, dv.y * dt, dv.z * dt, float(0.0f));

  q *= *this;

  x += q.x * 0.5f;
  y += q.y * 0.5f;
  z += q.z * 0.5f;
  w += q.w * 0.5f;
  NormalizeThis();

}

Quaternion Quaternion::RotateTo(const Vector3 & v1, const Vector3 & v2)
{
  //float k_cos_theta = v1.Dot(v2);//dot(u, v);
  //float k = sqrt(v1.LengthSq() * v2.LengthSq());

  //if (abs(k_cos_theta / k + 1.0f) <= 0.0000001f)
  //{
  //  // 180 degree rotation around any orthogonal vector
  //  printf("parallel vectors! ");
  //  Vector3 up(0, 1, 0);
  //  Vector3 right = up.Cross(v1);
  //  if(v1.x *v2.x >= 0.f  && v1.y * v2.y >= 0.f && v1.z * v2.z >= 0.f)
  //    return Quaternion(0, -up.Normalize());

  //  return Quaternion(PI, -up.Normalize());
  //}
  //Quaternion q(k_cos_theta + k, v1.Cross(v2));
  //q.NormalizeThis();
  //return q;
  Quaternion q;
  Vector3 c = v1.Cross(v2);//cross(v1, v2);
  q.x = c.x;
  q.y = c.y;
  q.z = c.z;

  if ((v1.LengthSq() - 1 <= 0.00001f) && (v2.LengthSq() - 1 < 0.00001f)) {
    q.w = 1 + v1.Dot(v2);//dot(v1, v2);
  }
  else {
    q.w = sqrt(v1.LengthSq() * v2.LengthSq()) + v1.Dot(v2);
  }
  q.NormalizeThis();
  return q;
}

bool RaytoAABB(const Vector3& CubeCentre, const Vector3& HalfWidths, const Vector3& RayOrigin, const Vector3& Ray, float& t)
{
  Vector3 MinPt = CubeCentre - HalfWidths;
  Vector3 MaxPt = CubeCentre + HalfWidths;

  // Check planes from -X, +X, -Y, +Y, -Z, +Z
  float PlanePts[6] = { MinPt.x, MaxPt.x, MinPt.y, MaxPt.y, MinPt.z, MaxPt.z };
  float RayOriginPts[6] = { RayOrigin.x, RayOrigin.x, RayOrigin.y, RayOrigin.y, RayOrigin.z, RayOrigin.z };
  float RayDirs[6] = { Ray.x, Ray.x, Ray.y, Ray.y, Ray.z, Ray.z };
  static const Vector3 PlaneNormals[6] = { { -1, 0, 0 }, { 1, 0, 0 }, { 0, -1, 0 }, { 0, 1, 0 }, { 0, 0, -1 }, { 0, 0, 1 } };

  float tExit = FLT_MAX;
  float tEnter = FLT_MIN;

  for (int i = 0; i < 6; ++i)
  {
    float T = (PlanePts[i] - RayOriginPts[i]) / RayDirs[i];

    // ray travels towards plane normal
    if (PlaneNormals[i].Dot(Ray) < 0)
    {
      tEnter = std::max(T, tEnter);
    }

    // ray travels away from plane normal
    else
    {
      tExit = std::min(T, tExit);
    }
  }

  if (tEnter < 0 || tExit < tEnter) return false;
  t = tEnter;
  return true;
}

bool PointInAABB(const Vector3 & Point, const Vector3 & CubeCentre, const Vector3 & HalfWidths)
{
  bool PositiveXPass = Point.x > CubeCentre.x - HalfWidths.x;
  bool NegativeXPass = Point.x < CubeCentre.x + HalfWidths.x;
  bool PositiveYPass = Point.y > CubeCentre.y - HalfWidths.y;
  bool NegativeYPass = Point.y < CubeCentre.y + HalfWidths.y;
  bool PositiveZPass = Point.z > CubeCentre.z - HalfWidths.z;
  bool NegativeZPass = Point.z < CubeCentre.z + HalfWidths.z;

  return PositiveXPass | NegativeXPass | PositiveYPass | NegativeYPass | PositiveZPass | NegativeZPass;
}

bool PointInSphere(const Vector3 & Point, const Vector3 & SphereCentre, float SphereRadius)
{
  float LengthFromCenterSq = (Point - SphereCentre).LengthSq();
  return LengthFromCenterSq < SphereRadius * SphereRadius;
}
