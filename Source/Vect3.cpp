// by Oleg Podsechin
#include "vect3.h"


void Vect3::operator *= (float scaler)	
{
  x *= scaler; y *= scaler; z *= scaler;
}

void Vect3::operator /= (float scaler)	
{
  x /= scaler; y /= scaler; z /= scaler;
}

void Vect3::operator += (const Vect3 &v) 
{
	x += v.x; y += v.y;	z += v.z;
}

void Vect3::operator -= (const Vect3 &v) 
{ 
	x -= v.x; y -= v.y;	z -= v.z;
}

void Vect3::Zero() 
{ 
  x = y = z = 0.f;
}

float Vect3::MagnitudeSquared() 
{
	return x * x + y * y + z * z;
}

float Vect3::Magnitude() 
{
	return sqrtf(MagnitudeSquared());
}

void Vect3::Normalize() 
{
	float m = Magnitude();
	if (m) { x /= m; y /= m; z /= m; }
}

Vect3 operator - (const Vect3 &v) 
{
	return Vect3(-v.x, -v.y, -v.z);
}

bool Vect3::operator == (const Vect3& v)
{
  return (x == v.x && y == v.y && z == v.z);
}

float Vect3::Distance(const Vect3 &a, const Vect3 &b)
{
  float x = a.x - b.x, y = a.y - b.y, z = a.z - b.z;
  return sqrtf(x * x + y * y + z * z);
}


Vect3 Vect3::Cross( const Vect3 &a, const Vect3 &b ) 
{
  return Vect3(	a.y * b.z - a.z * b.y,
				        a.z * b.x - a.x * b.z,
				        a.x * b.y - a.y * b.x );
}


float Vect3::Dot(const Vect3 &a, const Vect3 &b) 
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vect3 operator + (const Vect3 &a, const Vect3 &b) 
{
  return Vect3(a.x + b.x, a.y + b.y, a.z + b.z);
}


Vect3 operator - (const Vect3 &a, const Vect3 &b) 
{
  return Vect3(a.x - b.x, a.y - b.y, a.z - b.z);
}

//

Vect3 operator * (const Vect3 &v, const Mat3 &m)
{
  return Vect3(	m._11*v.x + m._21*v.y + m._31*v.z,
				        m._12*v.x + m._22*v.y + m._32*v.z,
				        m._13*v.x + m._23*v.y + m._33*v.z);
}


Vect3 operator * (const Vect3 &v, const Mat4 &m)
{
  return Vect3(	m._11 * v.x + m._21 * v.y + m._31 * v.z + m._41,
				        m._12 * v.x + m._22 * v.y + m._32 * v.z + m._42,
				        m._13 * v.x + m._23 * v.y + m._33 * v.z + m._43);
}

// same as above 2, but reversed

Vect3 operator * (const Mat3 &m, const Vect3 &v)
{
  return Vect3(	m._11*v.x + m._21*v.y + m._31*v.z,
				        m._12*v.x + m._22*v.y + m._32*v.z,
				        m._13*v.x + m._23*v.y + m._33*v.z);
}


Vect3 operator * (const Mat4 &m, const Vect3 &v)
{
  return Vect3(	m._11 * v.x + m._21 * v.y + m._31 * v.z + m._41,
				        m._12 * v.x + m._22 * v.y + m._32 * v.z + m._42,
				        m._13 * v.x + m._23 * v.y + m._33 * v.z + m._43);
}


Vect3 operator * (const Vect3 &v, float s)
{
	return Vect3(v.x * s, v.y * s, v.z * s);
}


Vect3 operator * (float s, const Vect3 &v)
{
	return Vect3(v.x * s, v.y * s, v.z * s);
}


Vect3 operator / (const Vect3 &v, float s)
{
	return Vect3( v.x / s, v.y / s, v.z / s );
}


