/*
 *  matrix.h
 *  proj3
 *
 *  Created by Michael Atwood on 6/11/08.
 *  Copyright 2008. All rights reserved.
 *
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vert3.h"

#define NUMCOLS 4
#define NUMROWS 4

typedef double mat4D[NUMROWS][NUMCOLS];

extern int  inverse_mat(double mat[][NUMCOLS], double inv[][NUMCOLS]);
extern void make_world_to_eye_mat(const vert3& eye, const vert3& coi, double  m[][NUMCOLS]);
extern void make_identmat(double m[][NUMCOLS]);
extern void transform_pt3D(double m[][NUMCOLS], const vert3& v, vert3& p);
extern void make_transmat(double dx, double dy, double dz, double m[][NUMCOLS]);
extern void copymat(double src[][NUMCOLS], double dest[][NUMCOLS]);
extern void matmult(double a[][NUMCOLS], double b[][NUMCOLS], double res[][NUMCOLS]);
extern const vec3 calc_face_norm(const vert3& p1, const vert3& p2, const vert3& p3);
extern const vec3 calc_face_norm_newell(const vert3 verts[], const int& num_verts);
extern const double dot(vec3 v, vec3 v2);


#endif