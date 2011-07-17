/*
 *  matrix.cpp
 *  proj3
 *
 *  Created by Michael Atwood on 6/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include <cmath>

#include "matrix.h"
#include "vert3.h"

/**
 * Calculates the inverse of the matrix
 */
int inverse_mat(double mat[][NUMCOLS], double inv[][NUMCOLS])
{
	int lp, i, j, k;
	static double wrk[NUMROWS][8];
	static double a, b;
	
	for(i=0; i<NUMROWS; i++) {           /* Set up matrices */
		for(j=0; j<NUMCOLS; j++) {
			wrk[i][j]=mat[i][j];
			wrk[i][j+NUMCOLS]=0.0;
		}
		wrk[i][i+NUMCOLS]=1.0;
	}
	for(lp=0; lp<4; lp++) {        /* Loop over all rows */
		a=0.0;
		j=(-1);
		for(i=lp; i<4; i++) {        /* Find largest non-zero element */
			b=wrk[i][lp];
			if(b<0.0)
				b=(-b);
			if(b>a) {
				a=b;
				j=i;
			}
		}
		if(j!=lp)	{                /* If not on diagonal, put it there */
			if(j<0)                    /* Singular if none found */
				return(0);
			else                       /* Exchange rows from lp to end */
				for(k=lp; k<8; k++) {
					a=wrk[j][k];
					wrk[j][k]=wrk[lp][k];
					wrk[lp][k]=a;
				}
		}
		a=wrk[lp][lp];               /* Normalize working row */
		for(i=lp; i<8; i++)
			wrk[lp][i]/=a;
		/*
		 Note: the column at lp is not cleared to zero since the
		 original matrix is destroyed anyway and only the second
		 half of the work matrix will be returned.
		 */
		
		for(i=lp+1; i<8; i++) {      /* Adjust rest of work space */
			b=wrk[lp][i];
			for(j=0; j<4; j++)	 /* One column at a time */
				if(j!=lp)
					wrk[j][i]-=wrk[j][lp]*b;
		}
	}
	
	for(i=0; i<NUMROWS; i++)	         /* Return result matrix */
		for(j=0; j<NUMCOLS; j++)
			inv[i][j]=wrk[i][j+NUMCOLS];
	
	return(1);
}

/**
 * Changes world coords to eye
 */
void make_world_to_eye_mat(const vert3& eye, const vert3& coi, double  m[][NUMCOLS])
{
	double a, b, c, r, p, pr;
	
	a = coi.x - eye.x;
	b = coi.y - eye.y;
	c = coi.z - eye.z;
	
	r = sqrt(a*a + b*b + c*c);
	p = sqrt(a*a + c*c);
	pr = p * r;
	
	m[0][0] = -c/p;
	m[0][1] = 0.0;
	m[0][2] = a/p;
	m[0][3] = eye.x*c/p - eye.z*a/p;
	
	m[1][0] = -a*b/pr;
	m[1][1] = p/r;
	m[1][2] = -b*c/pr;
	m[1][3] = eye.x*a*b/pr - eye.y*p/r + eye.z*b*c/pr;
	
	m[2][0] = a/r;
	m[2][1] = b/r;
	m[2][2] = c/r;
	m[2][3] = -eye.x*a/r - eye.y*b/r - eye.z*c/r;
	
	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}

/**
 * Makes the ident mat
 */
void make_identmat(double I[][NUMCOLS])
{
	I[0][0] = 1.0; I[0][1] = 0.0; I[0][2] = 0.0; I[0][3] = 0.0;
	I[1][0] = 0.0; I[1][1] = 1.0; I[1][2] = 0.0; I[1][3] = 0.0;
	I[2][0] = 0.0; I[2][1] = 0.0; I[2][2] = 1.0; I[2][3] = 0.0;
	I[3][0] = 0.0; I[3][1] = 0.0; I[3][2] = 0.0; I[3][3] = 1.0;
}

/*
 This routine is only to be used for scale, rotate, and translate.
 (e.g. World to Eye transform)
 */
void transform_pt3D(double m[][NUMCOLS], const vert3& v, vert3& p)
{
	p.x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3];
	p.y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3];
	p.z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3];
}

/**
 * Makes the translation matrix based on the vector
 */
void make_transmat(double dx, double dy, double dz, double m[][NUMCOLS])
{
	m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = dx;
	m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = dy;
	m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = dz;
	m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
}

/**
 * Copies a matrix
 */
void copymat(double src[][NUMCOLS], double dest[][NUMCOLS])
{
	int r, c;
	
	for (r = 0; r < NUMROWS; r++)
		for (c = 0; c < NUMCOLS; c++)
			dest[r][c] = src[r][c];
}

/**
 * Doing a copymat so that one can make the call, "matmult(a, b, a)".
 */
void matmult(double a[][NUMCOLS], double b[][NUMCOLS], double res[][NUMCOLS])
{
	int    r, c, i;
	mat4D  prod;
	
	for (r = 0; r < NUMROWS; r++)
		for (c = 0; c < NUMCOLS; c++) {
			prod[r][c] = 0.0;
			for (i = 0; i < NUMCOLS; i++)
				prod[r][c] += a[r][i]*b[i][c];
		}
	copymat(prod, res);
}

/** 
 * Calculates the OUTWARD face normal given 3 points on the face.  I am
 * assuming a clockwise orientation of the face vertices (when facing
 * outside of face).
 */
const vec3 calc_face_norm(const vert3& p1, const vert3& p2, const vert3& p3)
{
	vec3 v1, v2;
		
	v1 = p2-p1;
	v2 = p3-p2;
	
	return v2 * v1;
}

const double dot(vec3 v, vec3 v2)
{
	double out = 0.0;
	
	out += v.x * v2.x;
	out += v.y * v2.y;
	out += v.z * v2.z;
	
	return out;
}

const vec3 calc_face_norm_newell(const vert3 verts[], const int& num_verts)
{
	vec3 norm;
	norm.x = 0; norm.y = 0; norm.z = 0;
	
	for(int i = 0; i < num_verts; i++)
	{
		vert3 u = verts[i];
		vert3 v = verts[(i + 1) % num_verts];
		
		norm.x += (u.y - v.y) * (u.z + v.z);
		norm.y += (u.z - v.z) * (u.x + v.x);
		norm.z += (u.x - v.x) * (u.y + v.y);
	}
	
	return norm;
}