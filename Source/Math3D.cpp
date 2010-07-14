// by Oleg Podsechin
#include "Math3D.h"


Mat3 Math3D::xyzRot(float theta, float phi, float gamma)
{
  float piover180 = PI / 180.0f;
	theta *= piover180; phi*= piover180; gamma *= piover180; 

	float sint = sinf(theta), cost = cosf(theta),
			  sinp = sinf(phi),   cosp = cosf(phi),
			  sing = sinf(gamma), cosg = cosf(gamma);

  return Mat3(cosp*cosg - sint*sinp*sing,  -cost*sing, sinp*cosg + sint*cosp*sing, 
	            cosp*sing + sint*sinp*cosg,	 cost*cosg, sinp*sing - sint*cosp*cosg,	
	            -cost*sinp,	sint, cost*cosp);
}

Mat4 Math3D::Perspective(float fov, float nearplane, float farplane, float aspect)
{
	float planed = farplane - nearplane;

	if( fabs(planed) < 0.01f )
		return Mat4::Identity();

	float halffov = 0.5f*fov;

	if( fabs(sin(halffov)) < 0.01f )
		return Mat4::Identity();
    
  float cot = cosf(halffov)/sinf(halffov);

	float w = aspect*cot;
	float h = 1.0f*cot;
	float Q = farplane/planed;

	return Mat4(w, 0.0f, 0.0f, 0.0f,
					    0.0f, h, 0.0f, 0.0f,
					    0.0f, 0.0f, Q, 1.0f,
					    0.0f, 0.0f, -Q*nearplane, 0.0f );
}


Mat4 Math3D::Ortho(float left, float top, float right, float bottom, float n, float f)
{
	return Mat4(	2/(right-left), 0, 0, 0,
					0, 2/(top-bottom), 0, 0,
					0, 0, 1/(f-n), 0,
					-(right+left)/(right-left),
					(bottom+top)/(bottom-top),
					-n/(f-n), 1);
}

Mat4 Math3D::LookAt(Vect3& lookat, Vect3& lookfrom, Vect3& up)
{
  Vect3 z = lookat - lookfrom;
  z.Normalize();

  // @ todo get rid of the annoying minus sign
  Vect3 x(Vect3::Cross(up, z));
  Vect3 y(Vect3::Cross(x, -z));

  return Mat4(x.x, x.y, x.z, 0,
              y.x, y.y, y.z, 0,
              z.x, z.y, z.z, 0,
              lookfrom.x, lookfrom.y, lookfrom.z, 1);
}
