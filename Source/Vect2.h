#pragma once

// by Oleg Podsechin

#include "Math3D.h"

struct Mat3;
struct Mat2;

struct Vect2 { 
	
	union {
		struct { float x, y;  };
		struct { float u, v;  };
		float data[2];
	};
	
	float& operator[](int a) { return data[a]; }
	operator float*() { return &x; }

	void operator *= (float scaler);
	void operator += (const Vect2 &p);
	void operator -= (const Vect2 &p);
	
  void Zero();

  bool operator == (const Vect2& v);


	Vect2(float a, float b) : x(a), y(b) {}
	Vect2() {}
};


Vect2 operator -(const Vect2 &p); 
Vect2 operator +(const Vect2 &a, const Vect2 &b);
Vect2 operator -(const Vect2 &a, const Vect2 &b); 
Vect2 operator * (const Vect2 &p, const Mat3 &m);
Vect2 operator * (const Vect2 &p, const Mat2 &m);
