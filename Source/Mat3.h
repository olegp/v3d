#pragma once

// by Oleg Podsechin

#include "Math3D.h"

struct Mat4;
struct Vect3;

struct Mat3
{
	union {        
    struct {             
      float _11, _12, _13;
      float _21, _22, _23;
      float _31, _32, _33;
    };

    float data[3][3];
	};

  const float* operator[] (int a) const;

	float * operator[] ( int a );

	operator float*();

  bool operator == (const Mat3& m);


	Mat3();
	
	Mat3(	float m11, float m12, float m13,
			  float m21, float m22, float m23,
			  float m31, float m32, float m33 );

  // chops off the extra 4x4 data and fits into a 3x3
	Mat3(const Mat4 &m);

	Mat3(float *d);

  Mat3(const Vect3& axis, float angle);

  bool Inverse(Mat3& inverse, float tolerance = 1e-06f) const;
  Mat3 Inverse (float tolerance = 1e-06f) const;

  void FromAxisAngle(const Vect3& axis, float angle);

  // Gram-Schmidt orthonormalization (applied to columns of rotation matrix)
  void Orthonormalize ();

  // returns a matrix for vector/matrix multiplication
  static Mat3 Star(const Vect3& v);


	// returns the matrix flipped along the diagonal
	static Mat3 Transpose(const Mat3& m);

  static Mat3 Identity();
  static Mat3 Zero();
};

Mat3 operator* (const Mat3& a, const Mat3& b);
Mat3 operator+ (const Mat3& a, const Mat3& b);
Mat3 operator* (const Mat3& a, float b);
Mat3 operator* (float a, const Mat3& b);




