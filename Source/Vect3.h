#pragma once

// by Oleg Podsechin

#include "Math3D.h"

struct Mat3;
struct Mat4;

struct Vect3
{
	union {
		struct {
			float x, y, z;
		};

		float data[3];
	};

  float& operator[](int a) { return data[a]; }
  operator float*() { return &x; }

	void operator *= (float scaler);
	void operator /= (float scaler);
	void operator += (const Vect3 &v);
	void operator -= (const Vect3 &v);

	void Zero();

  float MagnitudeSquared();
	float Magnitude();
	void Normalize();

  bool operator == (const Vect3& v);

	Vect3(void) { }
	Vect3(float a, float b, float c) : x(a), y(b), z(c) { }
	Vect3(float *d)	{ memcpy(data, d, sizeof(data)); }

  static float Distance(const Vect3 &a, const Vect3 &b);
  static Vect3 Cross( const Vect3 &a, const Vect3 &b );
  static float Dot( const Vect3 &a, const Vect3 &b );

};

Vect3 operator - (const Vect3 &v);

Vect3 operator + (const Vect3 &a, const Vect3 &b);

Vect3 operator - (const Vect3 &a, const Vect3 &b);

Vect3 operator * (const Mat3 &m, const Vect3 &v);
Vect3 operator * (const Vect3 &v, const Mat3 &m);
Vect3 operator * (const Mat4 &m, const Vect3 &v);

Vect3 operator * (const Vect3 &v, float s);
Vect3 operator * (float s, const Vect3 &v);
Vect3 operator / (const Vect3 &v, float s);
