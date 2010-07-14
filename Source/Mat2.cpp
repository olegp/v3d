// by Oleg Podsechin
#include "mat2.h"


float * Mat2::operator[] ( int a ) 
{ 
  return data[a]; 
}

Mat2::operator float*() 
{ 
  return &_11; 
}

Mat2::Mat2() 
{
}
	
Mat2::Mat2(float *d) { 
  memcpy(&_11, d, sizeof(Mat2)); 
}

Mat2::Mat2(float m11, float m12, float m21, float m22) : _11(m11), _12(m12),	_21(m21), _22(m22)
{
}

bool Mat2::operator == (const Mat2& m)
{
  return memcmp(data, m.data, sizeof(float) * 4) == 0;
}


Mat2 Mat2::Identity()
{
  return Mat2(1, 0, 
              0, 1);
}


Mat2 Mat2::Zero()
{
  return Mat2(0, 0, 
              0, 0);
}

Mat2::Mat2(const Mat3& m) : _11(m._11), _12(m._12),	_21(m._21), _22(m._22)
{}

Mat2 operator* (const Mat2& a, const Mat2& b)
{
  return Mat2(
	  a._11 * b._11 + a._12 * b._21,
	  a._11 * b._12 + a._12 * b._22,

	  a._21 * b._11 + a._22 * b._21,
	  a._21 * b._12 + a._22 * b._22 );
}


Mat2 Mat2::Rot(float alpha)
{
	float sina = sinf(alpha), cosa = cosf(alpha);
	return Mat2( cosa, -sina, sina, cosa );
}


Mat2 Mat2::Transpose(const Mat2& m)	{
	return Mat2(m._11, m._21, m._12, m._22);
}
