/*
 *  gentube.cpp
 *  proj3
 *
 *  Created by Michael Atwood on 6/9/08.
 *  Copyright 2008. All rights reserved.
 *
 *  Creates a spiral tube using a frenet frame approach.
 */

#include <iostream>
#include <cmath>
#include <list>

#include "matrix.h"

#include "dobj.h"

using namespace std;

const int   SCREENWIDTH  = 640;
const int   SCREENHEIGHT = 480;
const int   NUMTWISTS    = 4;
const float DEG2RAD      = M_PI/180;
const float b            = 0.2;

#define POLY_FACES 4

dobj dout;

vert3 poly[POLY_FACES];

/**
 *
 */
void genPoly()
{
	poly[0].x = -.1; poly[0].y = .1; poly[0].z = 0;
	poly[1].x = .1; poly[1].y = .1; poly[1].z = 0;
	poly[2].x = .1; poly[2].y = -.1; poly[2].z = 0;
	poly[3].x = -.1; poly[3].y = -.1; poly[3].z = 0;
}

/**
 *
 */
void tanvec(float irad, vec3& v)
{
	float m = 1.0/(sqrt(1+b*b));
	v.x = -sin(irad) * m;
	v.y = cos(irad) * m;
	v.z = b*m;
}

/**
 *
 */
void nvec(float irad, vec3& v)
{
	float m = 1.0/(sqrt(1+b*b));
	v.x = b*sin(irad) * m;
	v.y = -b*cos(irad) * m;
	v.z = m;
}

/**
 *
 */
void genTube()
{
	float irad;
	vec3 tan, n, a;
	int numplanes = 0;
	
	genPoly();
	
	for (int i = 0; i < NUMTWISTS*360; i++) {
	//for(int i = 0; i < 200; i++){
		if(i%10 == 0)
		{
			mat4D mGL, trans, mfin;
			vert3 tvec;
			
			irad = i*DEG2RAD;
			
			tanvec(irad, tan);
			
			nvec(irad, n);
			
			tan.normalize();
			n.normalize();
			a = n * tan;
			
			mGL[0][0] =   a.x; mGL[0][1]   =   a.y; mGL[0][2] =   a.z; mGL[0][3] = 0;
			mGL[1][0] =   n.x; mGL[1][1]   =   n.y; mGL[1][2] =   n.z; mGL[1][3] = 0;
			mGL[2][0] =  -tan.x; mGL[2][1] =  -tan.y; mGL[2][2] =  -tan.z; mGL[2][3] = 0;
			mGL[3][0] =     0; mGL[3][1] =     0; mGL[3][2] =     0; mGL[3][3] = 1;
			
			make_transmat(-cos(irad), -sin(irad), -b*irad, trans);
			
			matmult(mGL, trans, mGL);
			
			inverse_mat(mGL, mfin);
			
			for(int j = 0; j<POLY_FACES; j++)
			{
				transform_pt3D(mfin, poly[j], tvec);
			
				dout.addVertex(tvec.x, tvec.y, tvec.z);
			}
		
			if(numplanes==0)
			{
				list<int> l;
				l.push_back(1);
				l.push_back(2);
				l.push_back(3);
				l.push_back(4);
				
				//dout.addFace(l);
			}
			//else if(i == NUMTWISTS*360-1) {}
			else
			{
				int currpoly = POLY_FACES * numplanes;
				for(int facenum=0; facenum<4; facenum++)
				{
					list<int> l;
					
					if(facenum == 3) //special case -- wrap around
					{
						l.push_back(currpoly+1+facenum);
						l.push_back(currpoly+1-POLY_FACES+facenum);
						l.push_back(currpoly+1-POLY_FACES);
						l.push_back(currpoly+1);
					}
					else
					{
						l.push_back(currpoly+1+facenum);
						l.push_back(currpoly+1-POLY_FACES+facenum);
						l.push_back(currpoly+2-POLY_FACES+facenum);
						l.push_back(currpoly+2+facenum);
					}
					
					dout.addFace(l);
				}
			}
			numplanes++;
		}		
	}
}

/**
 * Main gentube.cpp
 */
int main (int argc, char * argv[]) {	
	genTube();
	
	dout.saveObj("test.d");
	
    return 0;
}
