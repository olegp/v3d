#pragma once

// by Oleg Podsechin

#include "Math3D.h"

struct Mat4
{
	union {        
		
		struct {             
		float _11, _12, _13, _14;
        float _21, _22, _23, _24;
        float _31, _32, _33, _34;
        float _41, _42, _43, _44;
		};


    float data[4][4];
	};

  const float* operator[] (int a) const;
	float* operator[] ( int a );
	operator float*();

  bool operator == (const Mat2& m);

	Mat4();
	
	Mat4(	float m11, float m12, float m13, float m14,
			  float m21, float m22, float m23, float m24,
			  float m31, float m32, float m33, float m34,
			  float m41, float m42, float m43, float m44 );

 	bool operator == (const Mat4& m); 


	Mat4(float *d);

	// expands the 3x3 matrix to 4x4 
	// based on identity, hence the 1 in position 4.4
	Mat4(const Mat3 &m);

	static Mat4 Transpose(const Mat4& m);
	
  // gets the position vector
  Vect3 GetPos();

  // sets the position vector
  void SetPos(const Vect3& v);

  Vect3 GetXAxis();
  void SetXAxis(const Vect3& v);

  Vect3 GetYAxis();
  void SetYAxis(const Vect3& v);

  Vect3 GetZAxis();
  void SetZAxis(const Vect3& v);


  static Mat4 Identity();
  static Mat4 Zero();


  // returns the inverse of a matrix with a normalized rotation kernel
	// ( the one with perpendicular vectors and all, that is )
  static bool InverseAffine(const Mat4& in, Mat4& out);

  static bool InverseOrthogonal(const Mat4& in, Mat4& out);
};


Mat4 operator* (const Mat4& a, const Mat4& b);
Mat4 operator* (const Mat4& a, float b);


