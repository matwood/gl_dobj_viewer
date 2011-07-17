/*
 *  dobj.cpp
 *  proj3
 *
 *  Created by Michael Atwood on 6/1/08.
 *  Copyright 2008. All rights reserved.
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "dobj.h"
#include "matrix.h"


dobj::dobj(): radius(0.0)
{
	isLoaded = false;
	
	//put a 'placeholder' vert in the list b/c the d files start with 1
	vert3* v = new vert3(0.0,0.0,0.0);
	verts.push_back(v);
	
	centroid.x=0.0;
	centroid.y=0.0;
	centroid.z=0.0;
	
	//bronze
	material_Ka[0] = 0.19125f;
	material_Ka[1] = 0.0735f;
	material_Ka[2] = 0.0225f;
	material_Ka[3] = 1.0f;
	
	material_Kd[0] = 0.7038f;
	material_Kd[1] = 0.27048f;
	material_Kd[2] = 0.0828f;
	material_Kd[3] = 1.0f;
	
	material_Ks[0] = 0.256777f;
	material_Ks[1] = 0.137622f;
	material_Ks[2] = 0.086014f;
	material_Ks[3] = 1.0f;
	
	material_Se = 12.8f;
	
	//chrome
	/*material_Ka[0] = 0.25f;
	material_Ka[1] = 0.25f;
	material_Ka[2] = 0.25f;
	material_Ka[3] = 1.0f;
	
	material_Kd[0] = 0.4f;
	material_Kd[1] = 0.4f;
	material_Kd[2] = 0.4f;
	material_Kd[3] = 1.0f;
	
	material_Ks[0] = 0.774597f;
	material_Ks[1] = 0.774597f;
	material_Ks[2] = 0.774597f;
	material_Ks[3] = 1.0f;
	
	material_Se = 76.8f;*/
	
	//polished silver
	/*material_Ka[0] = 0.23125f;
	material_Ka[1] = 0.23125f;
	material_Ka[2] = 0.23125f;
	material_Ka[3] = 1.0f;
	
	material_Kd[0] = 0.2775f;
	material_Kd[1] = 0.2775f;
	material_Kd[2] = 0.2775f;
	material_Kd[3] = 1.0f;
	
	material_Ks[0] = 0.773911f;
	material_Ks[1] = 0.773911f;
	material_Ks[2] = 0.773911f;
	material_Ks[3] = 1.0f;
	
	material_Se = 89.6f;*/
}

dobj::~dobj()
{
	//clean up the memory used by the verts
	while(verts.size() > 0)
	{
		vert3* v = verts.back();
		verts.pop_back();
		
		delete(v);
	}
}

/**
 * Function that draws this .d obj
 */
void dobj::draw(bool fSmooth)
{
	if(isLoaded)
	{
		//use the same material for all the faces
		glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
		glMaterialf(GL_FRONT, GL_SHININESS, material_Se);
		
		if(fSmooth)
		{
			for(int i = 0; i < faces.size(); i++)
			{
				glBegin(GL_POLYGON);
				
				list<int> face = faces[i];
				list<int>::iterator iter = face.begin();
				while(iter != face.end())
				{
					vert3* v = verts[*iter];
					vec3 vnorm = vert_norms[*iter];
					
					glNormal3d(vnorm.x, vnorm.y, vnorm.z);
					glVertex3d(v->x, v->y, v->z);
					
					iter++;
				}
				
				glEnd();
			}
		}
		else
		{
			for(int i = 0; i < faces.size(); i++)
			{
				glBegin(GL_POLYGON);
				
				//get the face normal
				list<int> face = faces[i];
				vec3 norm = face_norm[i];
				glNormal3d(norm.x, norm.y, norm.z);
				
				list<int>::iterator iter = face.begin();
				while(iter != face.end())
				{
					vert3* v = verts[*iter];
					glVertex3d(v->x, v->y, v->z);
					iter++;
				}
				
				glEnd();
			}
		}
	}
}


/**
 * Function that draws this .d obj
 */
void dobj::drawVerts()
{
	if(isLoaded)
	{
		glBegin(GL_POINTS);
		
		glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
		
		for(int i = 1; i < verts.size(); i++)
		{
			vert3* v = verts[i];
			glVertex3d(v->x, v->y, v->z);
		}
		
		glEnd();
	}
}

/**
 * Loads an object from file
 * probably need to move this to a 'builder' class
 */
void dobj::loadObj(const string& path)
{
	string line;
	
	//open the file for reading
	ifstream dfile (path.c_str());
	
	if(dfile.is_open())
	{
		if(verifyObjFile(dfile))
		{
			loadVerts(dfile);
			loadFaces(dfile);
			
			dfile.close();
			
			loadMetadata();
			
			isLoaded = true;
		}
		else
		{
			cout << "File does not appear to be a .d obj file." << endl;
		}
	}
	else
	{
		//there was some error opening the file
		cout << "Error opening the .d file: " << path.c_str() << endl;
	}
}

/**
 * Function loads the verts from the .d file into internal memory structures
 * It assumes that the file point is sitting on the line of the first verts
 */
void dobj::loadVerts(ifstream& objfile)
{
	string line;
	
	for(int i = 0; i < num_verts; i++)
	{
		getline(objfile, line, '\n');
		if(!line.empty())
		{
			istringstream buf(line);
			GLdouble x, y, z;
			buf >> x;
			buf >> y;
			buf >> z;
			
			vert3* v = new vert3(x, y, z);
			verts.push_back(v);
		}
	}
}

/**
 * Adds a vertex to the list
 */
void dobj::addVertex(GLdouble x, GLdouble y, GLdouble z)
{
	vert3* v = new vert3(x, y, z);
	verts.push_back(v);
}

/**
 * Function loads the faces from the .d file.  It assumes
 * that the file point is sitting at the first face line
 */
void dobj::loadFaces(ifstream& objfile)
{
	string line;
	vector<list<int> > vert_face (verts.size());
	
	for(int i = 0; i < num_faces; i++)
	{
		getline(objfile, line, '\n');
		if(!line.empty())
		{
			list<int> face;
			istringstream buf(line);
			int numvals;
			
			buf >> numvals;
			for(int j = 0; j < numvals; j++)
			{
				int temp;
				buf >> temp;
				face.push_back(temp);
				
				//add the face to the vert list
				vert_face[temp].push_back(i);
			}
			
			//just grab the first 3 verts to gen the face normal
			list<int>::iterator iter = face.begin();
			
			vert3* v1 = verts[*iter]; iter++;
			vert3* v2 = verts[*iter]; iter++;
			vert3* v3 = verts[*iter];
			
			vec3 norm = calc_face_norm(*v3, *v2, *v1);
			norm.normalize();
			
			face_norm.push_back(norm);
			
			faces.push_back(face);
		}
	}
	
	//at this point we should have a list of verts and the faces they belong to
	//lets avg the normals
	for(int i = 0; i < vert_face.size(); i++)
	{
		GLdouble x, y, z;
		
		x = 0.0;
		y = 0.0;
		z = 0.0;
		
		//cout << "Vert: " << i << " Faces:";
		list<int> face_list = vert_face[i];
		list<int>::iterator iter = face_list.begin();
		while(iter != face_list.end())
		{
			vec3 fvec = face_norm[*iter];
			
			x += fvec.x;
			y += fvec.y;
			z += fvec.z;
			
			//cout << " " << *iter;
			iter++;
		}
		
		cout << endl;
		
		vec3 v(x, y, z);
		//v.toString();
		v.normalize();
		vert_norms.push_back(v);
	}
}

/**
 * Adds a face to the list
 */
void dobj::addFace(list<int> face)
{
	faces.push_back(face);
}

/**
 * Calculates/loads additional object metadata
 */
void dobj::loadMetadata()
{

	calcCentroid();
	
	for(int i = 1; i < verts.size(); i++)
	{
		vert3* v = verts[i];
		vec3 vec = centroid - *v;
		
		GLdouble mag = vec.magnitude();
		if(radius < mag)
		{
			radius = mag;
		}
	}
}

/**
 * Calculates the centroid of the object
 */
void dobj::calcCentroid()
{
	GLdouble sumx, sumy, sumz;
	
	sumx=0.0;
	sumy=0.0;
	sumz=0.0;
	
	for(int i = 1; i < verts.size(); i++)
	{
		vert3* v = verts[i];
		
		sumx+=v->x;
		sumy+=v->y;
		sumz+=v->z;
	}
	
	centroid.x = sumx/num_verts;
	centroid.y = sumy/num_verts;
	centroid.z = sumz/num_verts;
}

/**
 * Verifies that the file is a .d obj file and loads the num_verts and num_faces vars
 */
bool dobj::verifyObjFile(ifstream& objfile)
{
	//read the header information
	string hline;
	getline(objfile, hline);
	
	if(hline.empty()) {return false;}
	
	istringstream buf;
    buf.str(hline);
	
	string head;
	buf >> head >> num_verts >> num_faces;
	//buf >> num_verts;
	//buf >> num_faces;
	
	return true;
}

/**
 * Saves the object in memory to a .d file format
 */
void dobj::saveObj(const string& path)
{	
	//open the file for reading
	ofstream dfile;
	dfile.open(path.c_str());
	
	dfile << "data " << verts.size() - 1 << " " << faces.size() << endl;
	
	for(int i = 1; i < verts.size(); i++)
	{
		vert3* v = verts[i];
		
		dfile << v->x << " " << v->y << " " << v->z << endl;
	}
	
	for(int i = 0; i < faces.size(); i++)
	{
		list<int> face = faces[i];
		
		dfile << face.size();
		
		list<int>::iterator iter = face.begin();
		while(iter != face.end())
		{
			dfile << " " << *iter; 
			iter++;
		}
		
		dfile << endl;
	}
	
	dfile.close();
}

/**
 * Debugging function outputs the loaded verts
 */
void dobj::printVerts()
{
	for(int i = 0; i < verts.size(); i++)
	{
		vert3* v = verts[i];
		
		cout << "X:" << v->x
			<< " Y:" << v->y
		<< " Z:" << v->z << endl;
	}
}

/**
 * Debugging function outputs the loaded faces
 */
void dobj::printFaces()
{
	for(int i = 0; i < faces.size(); i++)
	{
		list<int> l = faces[i];
		
		cout << "Face size:" << l.size() << " Verts: ";
		
		list<int>::iterator iter = l.begin();
		while(iter != l.end())
		{
			cout << *iter << " "; 
			iter++;
		}
		
		cout << endl;
	}
}

/**
 * Debugging function prints out obj metadata
 */
void dobj::printMetadata()
{
	cout << "Centroid: x=" << centroid.x
		<< " y=" << centroid.y
		<< " z=" << centroid.z
	<< " Radius:" << radius << endl;
}