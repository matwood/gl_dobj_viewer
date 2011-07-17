/*
 *  vec3.cpp
 *  proj3
 *
 *  Created by Michael Atwood on 5/30/08.
 *  Copyright 2008. All rights reserved.
 *
 */
#include <iostream>
#include <cmath>

#include "vec3.h"

/**
 * Returns the magnitude of the vector
 */
GLdouble vec3::magnitude()
{
	return sqrt((x*x)+(y*y)+(z*z));
}

/**
 * Normalizes the vector
 */
void vec3::normalize()
{
	GLdouble length = magnitude();
	
	if (length == 0)
		return;
		
	x /= length;
	y /= length;
	z /= length;
}

/**
 * Cross product operator overloading
 */
const vec3 operator*(const vec3& v1, const vec3& v2)
{
	vec3 out;
	
	out.x = v1.y * v2.z - v1.z * v2.y;
	out.y = v1.z * v2.x - v1.x * v2.z;
	out.z = v1.x * v2.y - v1.y * v2.x;
	
	return out;
}

const vec3 operator+(const vec3& v1, const vec3& v2)
{
	vec3 out;
	
	out.x = v1.x + v2.x;
	out.y = v1.y + v2.y;
	out.z = v1.z + v2.z;
	
	return out;
}

/**
 * Debug out
 */
void vec3::toString()
{
	std::cout << "x:" << x
	<< " y:" << y
	<< " z:" << z
	<< std::endl;
}