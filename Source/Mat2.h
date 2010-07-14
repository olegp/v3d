#pragma once

// by Oleg Podsechin

#include "Math3D.h"

struct Mat2
{
	union {        
		
		struct {             
			float _11, _12;
			float _21, _22;
		};

    float data[2][2];
	};


	float * operator[] ( int a );
	operator float*();

	Mat2();
	
  Mat2(float *d);
  Mat2(float m11, float m12, float m21, float m22);
	Mat2(const Mat3& m);

  bool operator == (const Mat2& m);


	// returns the matrix flipped along the diagonal
	static Mat2 Transpose(const Mat2& m);

  // creates a (counter clockwise) rotational matrix 
  static Mat2 Rot(float alpha);

  static Mat2 Identity();
  static Mat2 Zero();
};


Mat2 operator* (const Mat2& a, const Mat2& b);

