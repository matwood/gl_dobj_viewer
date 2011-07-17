/*
 *  vec3.h
 *  proj3
 *
 *  Created by Michael Atwood on 5/30/08.
 *  Copyright 2008. All rights reserved.
 *
 */

#ifndef __vec3_h__
#define __vec3_h__

#include <GLUT/glut.h>

class vec3
{
	public:
	vec3(): x(0.0), y(0.0), z(0.0) {};
	vec3(GLdouble _x, GLdouble _y, GLdouble _z): x(_x), y(_y), z(_z) {};
	GLdouble x;
	GLdouble y;
	GLdouble z;
	
	GLdouble magnitude();
	void normalize();
	
	friend const vec3 operator*(const vec3& v1, const vec3& v2);
	friend const vec3 operator+(const vec3& v1, const vec3& v2);
	
	void toString(); //debugging outputs to console
};

#endif