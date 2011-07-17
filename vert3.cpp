/*
 *  vert3.cpp
 *  proj3
 *
 *  Created by Michael Atwood on 6/1/08.
 *  Copyright 2008. All rights reserved.
 *
 */

#include <iostream>

#include "vert3.h"

using namespace std;

const vec3 operator-(const vert3& left, const vert3& right)
{
	return vec3(left.x-right.x, left.y-right.y, left.z-right.z);
}

/**
 * Outputs the vertex to the standard console
 */
void vert3::toString()
{
	cout << "x:" << x
		 << " y:" << y
		 << " z:" << z
	<< endl;
}