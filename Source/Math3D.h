#pragma once

// by Oleg Podsechin

#include <math.h>
#include <memory.h>

#ifndef PI
#define PI 3.141592653589793238f
#endif

#include "Vect2.h"
#include "Vect3.h"

#include "Mat2.h"
#include "Mat3.h"
#include "Mat4.h"

struct Vect3;

class Math3D
{
public:
	// returns the concatenated x, y, z rotation matrix
	// not very efficient if rotating about only one axis
	static Mat3 xyzRot(float theta, float phi, float gamma);

  // returns a perspective projection matrix
  static Mat4 Perspective(float fov, float nearplane, float farplane, float aspect);
	
  // returns an orthogonal projection matrix
  static Mat4 Ortho(float left, float top, float right, float bottom, float n = -1, float f = 1);
  
  // returns a "look at" matrix, that is one that has its coordinate system
  // centered at lookfrom, pointing in the direction of lookat with up being the general up direction
  // up must be normalized
  static Mat4 LookAt(Vect3& lookat, Vect3& lookfrom, Vect3& up);
};