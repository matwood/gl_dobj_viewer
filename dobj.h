/*
 *  dobj.h
 *  proj3
 *
 *  Created by Michael Atwood on 6/1/08.
 *  Copyright 2008. All rights reserved.
 *
 */

#ifndef __dobj_h__
#define __dobj_h__

#include <fstream>
#include <vector>
#include <string>
#include <list>

#include "vert3.h"
#include "vec3.h"

using namespace std;

class dobj
{
public:
	dobj();
	~dobj();
	
	void draw(bool fSmooth);
	void drawVerts();
	void loadObj(const string& path);
	void saveObj(const string& path);
	
	void addVertex(GLdouble x, GLdouble y, GLdouble z);
	void addFace(list<int> face);
	
	vert3 centroid;
	GLdouble radius;
	
	//debug funcs
	void printVerts();
	void printFaces();
	void printMetadata();
private:
	void loadVerts(ifstream& objfile);
	void loadFaces(ifstream& objfile);
	void loadMetadata();
	void calcCentroid();
	bool verifyObjFile(ifstream& objfile);
	
	bool isLoaded;
	vector<vert3*> verts;
	vector<vec3> vert_norms;
	
	vector<list<int> > faces;
	vector<vec3> face_norm;
	
	int num_verts;
	int num_faces;
	
	//material type of object
	GLfloat material_Ka[4];
	GLfloat material_Kd[4];
	GLfloat material_Ks[4];
	GLfloat material_Se;
};

#endif