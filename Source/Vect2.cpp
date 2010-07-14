// by Oleg Podsechin
#include "Vect2.h"

void Vect2::operator *= (float scaler)	
{ 
  x *= scaler; y *= scaler; 
}

void Vect2::operator += (const Vect2 &p) 
{ 
  x += p.x; y += p.y; 
}

void Vect2::operator -= (const Vect2 &p) 
{ 
  x -= p.x; y -= p.y; 
}

void Vect2::Zero() 
{ 
  x = y = 0.f; 
}

Vect2 operator -(const Vect2 &p) 
{ 
  return Vect2(-p.x, -p.y); 
}

Vect2 operator +(const Vect2 &a, const Vect2 &b) 
{ 
  return Vect2(a.x + b.x, a.y + b.y); 
}

Vect2 operator -(const Vect2 &a, const Vect2 &b) 
{ 
  return Vect2(a.x - b.x, a.y - b.y); 
}

Vect2 operator *(const Vect2 &p, const Mat3 &m)
{
return Vect2(	m._11*p.x + m._21*p.y + m._31,	m._12*p.x + m._22*p.y + m._32 );
}

Vect2 operator *(const Vect2 &p, const Mat2 &m)
{
return Vect2(	m._11*p.x + m._21*p.y, m._12*p.x + m._22*p.y );
}

bool Vect2::operator == (const Vect2& v)
{
  return (x == v.x && y == v.y);
}
