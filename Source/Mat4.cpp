// by Oleg Podsechin
#include "mat4.h"

const float* Mat4::operator[] (int a) const
{
  return data[a]; 
}

float* Mat4::operator[] ( int a ) { return data[a]; }
Mat4::operator float*() { return &_11; }

Mat4::Mat4() {}

Mat4::Mat4(	float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44 ) :
	    _11(m11), _12(m12), _13(m13), _14(m14),
	    _21(m21), _22(m22), _23(m23), _24(m24),
	    _31(m31), _32(m32), _33(m33), _34(m34),
	    _41(m41), _42(m42), _43(m43), _44(m44)
	    {}


Mat4::Mat4(float *d)	{ memcpy(&_11, d, sizeof(Mat4)); }


bool Mat4::operator == (const Mat4& m)
{
  return memcmp(data, m.data, sizeof(float) * 16) == 0;
}


Mat4 Mat4::Identity()
{
  return Mat4(1, 0, 0, 0,
			        0, 1, 0, 0,
			        0, 0, 1, 0,
			        0, 0, 0, 1);
}


Mat4 Mat4::Zero()
{
  return Mat4(0, 0, 0, 0,
			        0, 0, 0, 0,
			        0, 0, 0, 0,
			        0, 0, 0, 0);
}

Vect3 Mat4::GetPos() 
{
  return Vect3(_41, _42 , _43);
}

void Mat4::SetPos(const Vect3& v) 
{
  _41 = v.x; 
  _42 = v.y; 
  _43 = v.z;
}


Vect3 Mat4::GetXAxis() 
{
  return Vect3(_11, _12 , _13);
}

void Mat4::SetXAxis(const Vect3& v) 
{
  _11 = v.x; 
  _12 = v.y; 
  _13 = v.z;
}

Vect3 Mat4::GetYAxis() 
{
  return Vect3(_21, _22 , _23);
}

void Mat4::SetYAxis(const Vect3& v) 
{
  _21 = v.x; 
  _22 = v.y; 
  _23 = v.z;
}

Vect3 Mat4::GetZAxis() 
{
  return Vect3(_31, _32 , _33);
}

void Mat4::SetZAxis(const Vect3& v) 
{
  _31 = v.x; 
  _32 = v.y; 
  _33 = v.z;
}

Mat4 Mat4::Transpose(const Mat4& m)
{
	return Mat4(m._11, m._21, m._31, m._41,
		          m._12, m._22, m._32, m._42,
		          m._13, m._23, m._33, m._43,
		          m._14, m._24, m._34, m._44);
}


Mat4::Mat4( const Mat3 &m ) :
	  _11(m._11), _12(m._12), _13(m._13), _14(0),
	  _21(m._21), _22(m._22), _23(m._23), _24(0),
	  _31(m._31), _32(m._32), _33(m._33), _34(0),
	  _41(0),   _42(0),   _43(0),   _44(1)
{}




Mat4 operator* (const Mat4& a, const Mat4& b) 
{
  return Mat4(
	  a._11 * b._11 + a._12 * b._21 + a._13 * b._31 + a._14 * b._41,
	  a._11 * b._12 + a._12 * b._22 + a._13 * b._32 + a._14 * b._42,
	  a._11 * b._13 + a._12 * b._23 + a._13 * b._33 + a._14 * b._43,
	  a._11 * b._14 + a._12 * b._24 + a._13 * b._34 + a._14 * b._44,
	  a._21 * b._11 + a._22 * b._21 + a._23 * b._31 + a._24 * b._41,
	  a._21 * b._12 + a._22 * b._22 + a._23 * b._32 + a._24 * b._42,
	  a._21 * b._13 + a._22 * b._23 + a._23 * b._33 + a._24 * b._43,
	  a._21 * b._14 + a._22 * b._24 + a._23 * b._34 + a._24 * b._44,
	  a._31 * b._11 + a._32 * b._21 + a._33 * b._31 + a._34 * b._41,
	  a._31 * b._12 + a._32 * b._22 + a._33 * b._32 + a._34 * b._42,
	  a._31 * b._13 + a._32 * b._23 + a._33 * b._33 + a._34 * b._43,
	  a._31 * b._14 + a._32 * b._24 + a._33 * b._34 + a._34 * b._44,
	  a._41 * b._11 + a._42 * b._21 + a._43 * b._31 + a._44 * b._41,
	  a._41 * b._12 + a._42 * b._22 + a._43 * b._32 + a._44 * b._42,
	  a._41 * b._13 + a._42 * b._23 + a._43 * b._33 + a._44 * b._43,
	  a._41 * b._14 + a._42 * b._24 + a._43 * b._34 + a._44 * b._44);
}

Mat4 operator* (const Mat4& a, float b)
{
  return Mat4(
	  a._11 * b, a._12 * b, a._13 * b, a._14 * b,
	  a._21 * b, a._22 * b, a._23 * b, a._24 * b,
	  a._31 * b, a._32 * b, a._33 * b, a._34 * b,
	  a._41 * b, a._42 * b, a._43 * b, a._44 * b);
}


bool Mat4::InverseOrthogonal(const Mat4& in, Mat4& out) 
{
	Mat3 rot = Mat3::Transpose(Mat3(in));
	Vect3 prev(in._41, in._42, in._43), pos = prev * rot;

	out = Mat4(rot);
  out.SetPos(-pos);
  return true;
}





// the code below is taken from Graphics Gems volume 2

/****
 *
 * InverseAffine()
 *
 * Computes the inverse of a 3D affine matrix; i.e. a matrix with a dimen-
 * sionality of 4 where the right column has the entries (0, 0, 0, 1).
 *
 * This procedure treats the 4 by 4 matrix as a block matrix and
 * calculates the inverse of one submatrix for a significant perform-
 * ance improvement over a general procedure that can invert any non-
 * singular matrix:
 *          --        --          --          --
 *          |          | -1       |    -1      |
 *          | A      0 |          |   A      0 |
 *    -1    |          |          |            |
 *   M   =  |          |     =    |     -1     |
 *          | C      1 |          | -C A     1 |
 *          |          |          |            |
 *          --        --          --          --
 *
 *  where     M is a 4 by 4 matrix,
 *            A is the 3 by 3 upper left submatrix of M,
 *            C is the 1 by 3 lower left submatrix of M.
 *
 * Input:
 *   in   - 3D affine matrix
 *
 * Output:
 *   out  - inverse of 3D affine matrix
 *
 * Returned value:
 *   true   if input matrix is nonsingular
 *   false  otherwise
 *
 ***/

bool Mat4::InverseAffine(const Mat4& in, Mat4& out)
{
float det_1;
float pos, neg, temp;

#define ACCUMULATE    \
  if (temp >= 0.0)  \
    pos += temp;  \
  else              \
    neg += temp;

#define PRECISION_LIMIT (1.0e-15)

  /*
   * Calculate the determinant of submatrix A and determine if the
   * the matrix is singular as limited by the double precision
   * floating-point data representation.
   */
  pos = neg = 0.0;
  temp =  in[0][0] * in[1][1] * in[2][2];
  ACCUMULATE
  temp =  in[0][1] * in[1][2] * in[2][0];
  ACCUMULATE
  temp =  in[0][2] * in[1][0] * in[2][1];
  ACCUMULATE
  temp = -in[0][2] * in[1][1] * in[2][0];
  ACCUMULATE
  temp = -in[0][1] * in[1][0] * in[2][2];
  ACCUMULATE
  temp = -in[0][0] * in[1][2] * in[2][1];
  ACCUMULATE
  det_1 = pos + neg;

  // Is the submatrix A singular?
  if ((det_1 == 0.0) || (fabs(det_1 / (pos - neg)) < PRECISION_LIMIT)) {
    // Matrix M has no inverse
    return false;

  } else {
    // Calculate inverse(A) = adj(A) / det(A)
    det_1 = 1.0f / det_1;
    out[0][0] = ( in[1][1] * in[2][2] - in[1][2] * in[2][1] ) * det_1;
    out[1][0] = - ( in[1][0] * in[2][2] - in[1][2] * in[2][0] ) * det_1;
    out[2][0] =   ( in[1][0] * in[2][1] - in[1][1] * in[2][0] ) * det_1;
    out[0][1] = - ( in[0][1] * in[2][2] - in[0][2] * in[2][1] ) * det_1;
    out[1][1] =   ( in[0][0] * in[2][2] - in[0][2] * in[2][0] ) * det_1;
    out[2][1] = - ( in[0][0] * in[2][1] - in[0][1] * in[2][0] ) * det_1;
    out[0][2] =   ( in[0][1] * in[1][2] - in[0][2] * in[1][1] ) * det_1;
    out[1][2] = - ( in[0][0] * in[1][2] - in[0][2] * in[1][0] ) * det_1;
    out[2][2] =   ( in[0][0] * in[1][1] - in[0][1] * in[1][0] ) * det_1;

    // Calculate -C * inverse(A)
    out[3][0] = - ( in[3][0] * out[0][0] + in[3][1] * out[1][0] + in[3][2] * out[2][0] );
    out[3][1] = - ( in[3][0] * out[0][1] + in[3][1] * out[1][1] + in[3][2] * out[2][1] );
    out[3][2] = - ( in[3][0] * out[0][2] + in[3][1] * out[1][2] + in[3][2] * out[2][2] );

    // Fill in last column
    out[0][3] = out[1][3] = out[2][3] = 0.0;
    out[3][3] = 1.0;

    return true;
  }
}
