// by Oleg Podsechin
#include "mat3.h"

const float* Mat3::operator[] (int a) const
{
  return data[a];
}

float * Mat3::operator[] ( int a ) { return data[a]; }

Mat3::operator float*() { return &_11; }


Mat3::Mat3() {}

Mat3::Mat3(	float m11, float m12, float m13,
			float m21, float m22, float m23,
			float m31, float m32, float m33 ) :
_11(m11), _12(m12), _13(m13),
_21(m21), _22(m22), _23(m23),
_31(m31), _32(m32), _33(m33)
{ }


Mat3::Mat3(float *d)	{ memcpy(&_11, d, sizeof(Mat3)); }


bool Mat3::operator == (const Mat3& m)
{
  return memcmp(data, m.data, sizeof(float) * 9) == 0;
}


Mat3 Mat3::Identity()
{
  return Mat3(1, 0, 0, 
              0, 1, 0, 
              0, 0, 1);
}

Mat3 Mat3::Zero()
{
  return Mat3(0, 0, 0, 
              0, 0, 0, 
              0, 0, 0);
}

Mat3::Mat3( const Mat4 &m ) :
	_11(m._11), _12(m._12), _13(m._13),
	_21(m._21), _22(m._22), _23(m._23),
	_31(m._31), _32(m._32), _33(m._33)

  {}


Mat3::Mat3(const Vect3& axis, float angle)
{
  FromAxisAngle(axis, angle);
}

Mat3 operator* (const Mat3& a, const Mat3& b)	
{
  return Mat3(
	  a._11 * b._11 + a._12 * b._21 + a._13 * b._31,
	  a._11 * b._12 + a._12 * b._22 + a._13 * b._32,
	  a._11 * b._13 + a._12 * b._23 + a._13 * b._33,
	  a._21 * b._11 + a._22 * b._21 + a._23 * b._31,
	  a._21 * b._12 + a._22 * b._22 + a._23 * b._32,
	  a._21 * b._13 + a._22 * b._23 + a._23 * b._33,
	  a._31 * b._11 + a._32 * b._21 + a._33 * b._31,
	  a._31 * b._12 + a._32 * b._22 + a._33 * b._32,
	  a._31 * b._13 + a._32 * b._23 + a._33 * b._33 );
}

Mat3 operator+ (const Mat3& a, const Mat3& b)	
{
  return Mat3(a._11 + b._11, a._12 + b._12, a._13 + b._13,
	            a._21 + b._21, a._22 + b._22, a._23 + b._23,
	            a._31 + b._31, a._32 + b._32, a._33 + b._33);
}

Mat3 operator* (const Mat3& a, float b)	
{
  return Mat3(
	  a._11 * b, a._12 * b, a._13 * b, 
    a._21 * b, a._22 * b, a._23 * b,
    a._31 * b, a._32 * b, a._33 * b );
}

Mat3 operator* (float a, const Mat3& b)	
{
  return Mat3(
	  b._11 * a, b._12 * a, b._13 * a, 
    b._21 * a, b._22 * a, b._23 * a,
    b._31 * a, b._32 * a, b._33 * a );
}


Mat3 Mat3::Transpose(const Mat3& m)	
{
	return Mat3(	m._11, m._21, m._31,
					m._12, m._22, m._32,
					m._13, m._23, m._33);
}

Mat3 Mat3::Star(const Vect3& v)
{
  return Mat3(0, -v.z, v.y, 
              v.z, 0, -v.x, 
              -v.y, v.x, 0);
}

// Invert a 3x3 using cofactors.  This is about 8 times faster than
// the Numerical Recipes code which uses Gaussian elimination.
// taken from www.magic-software.com
bool Mat3::Inverse(Mat3& inverse, float tolerance) const
{
  inverse[0][0] = data[1][1]*data[2][2] - data[1][2]*data[2][1];
  inverse[0][1] = data[0][2]*data[2][1] - data[0][1]*data[2][2];
  inverse[0][2] = data[0][1]*data[1][2] - data[0][2]*data[1][1];
  inverse[1][0] = data[1][2]*data[2][0] - data[1][0]*data[2][2];
  inverse[1][1] = data[0][0]*data[2][2] - data[0][2]*data[2][0];
  inverse[1][2] = data[0][2]*data[1][0] - data[0][0]*data[1][2];
  inverse[2][0] = data[1][0]*data[2][1] - data[1][1]*data[2][0];
  inverse[2][1] = data[0][1]*data[2][0] - data[0][0]*data[2][1];
  inverse[2][2] = data[0][0]*data[1][1] - data[0][1]*data[1][0];

  float fDet = data[0][0]*inverse[0][0] + data[0][1]*inverse[1][0]+ data[0][2]*inverse[2][0];

  if(fabsf(fDet) <= tolerance) return false;

  float fInvDet = 1.0f/fDet;
  for (int iRow = 0; iRow < 3; iRow++) {
    for(int iCol = 0; iCol < 3; iCol++)
      inverse[iRow][iCol] *= fInvDet;
  }

  return true;
}

Mat3 Mat3::Inverse (float tolerance) const
{
  Mat3 inverse;
  Inverse(inverse, tolerance);
  return inverse;
}

void Mat3::FromAxisAngle (const Vect3& axis, float angle)
{
  float fCos = cosf(angle);
  float fSin = sinf(angle);
  float fOneMinusCos = 1.0f-fCos;
  float fX2 = axis.x*axis.x;
  float fY2 = axis.y*axis.y;
  float fZ2 = axis.z*axis.z;
  float fXYM = axis.x*axis.y*fOneMinusCos;
  float fXZM = axis.x*axis.z*fOneMinusCos;
  float fYZM = axis.y*axis.z*fOneMinusCos;
  float fXSin = axis.x*fSin;
  float fYSin = axis.y*fSin;
  float fZSin = axis.z*fSin;
  
  data[0][0] = fX2*fOneMinusCos+fCos;
  data[0][1] = fXYM-fZSin;
  data[0][2] = fXZM+fYSin;
  data[1][0] = fXYM+fZSin;
  data[1][1] = fY2*fOneMinusCos+fCos;
  data[1][2] = fYZM-fXSin;
  data[2][0] = fXZM-fYSin;
  data[2][1] = fYZM+fXSin;
  data[2][2] = fZ2*fOneMinusCos+fCos;
}

void Mat3::Orthonormalize ()
{
  // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
  // M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
  //
  //   q0 = m0/|m0|
  //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
  //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
  //
  // where |V| indicates length of vector V and A*B indicates dot
  // product of vectors A and B.

  // compute q0
  float fInvLength = 1.0f/sqrtf(data[0][0]*data[0][0]
      + data[1][0]*data[1][0] + data[2][0]*data[2][0]);

  data[0][0] *= fInvLength;
  data[1][0] *= fInvLength;
  data[2][0] *= fInvLength;

  // compute q1
  float fDot0 = data[0][0]*data[0][1] +
                data[1][0]*data[1][1] +
                data[2][0]*data[2][1];

  data[0][1] -= fDot0*data[0][0];
  data[1][1] -= fDot0*data[1][0];
  data[2][1] -= fDot0*data[2][0];

  fInvLength = 1.0f/sqrtf(data[0][1]*data[0][1] +
      data[1][1]*data[1][1] +
      data[2][1]*data[2][1]);

  data[0][1] *= fInvLength;
  data[1][1] *= fInvLength;
  data[2][1] *= fInvLength;

  // compute q2
  float fDot1 = data[0][1]*data[0][2] +
                data[1][1]*data[1][2] +
                data[2][1]*data[2][2];

  fDot0 = data[0][0]*data[0][2] +
          data[1][0]*data[1][2] +
          data[2][0]*data[2][2];

  data[0][2] -= fDot0*data[0][0] + fDot1*data[0][1];
  data[1][2] -= fDot0*data[1][0] + fDot1*data[1][1];
  data[2][2] -= fDot0*data[2][0] + fDot1*data[2][1];

  fInvLength = 1.0f/sqrtf(data[0][2]*data[0][2] +
      data[1][2]*data[1][2] +
      data[2][2]*data[2][2]);

  data[0][2] *= fInvLength;
  data[1][2] *= fInvLength;
  data[2][2] *= fInvLength;
}

