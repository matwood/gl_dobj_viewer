/*
 *  vert3.h
 *  proj3
 *
 *  Created by Michael Atwood on 6/1/08.
 *  Copyright 2008. All rights reserved.
 *
 */

#ifndef __vert3_h__
#define __vert3_h__

#include <GLUT/glut.h>

#include "vec3.h"

class vert3
	{
	public:
		vert3(): x(0.0), y(0.0), z(0.0) {};
		vert3(GLdouble _x, GLdouble _y, GLdouble _z): x(_x), y(_y), z(_z) {};
		GLdouble x;
		GLdouble y;
		GLdouble z;
		
		friend const vec3 operator-(const vert3& left, const vert3& right);
		
		void toString(); //debugging...outputs vert to the console
	};

#endif