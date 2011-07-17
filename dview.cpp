/**
 * Michael Atwood
 */

#include <iostream>
#include <cmath>

#include <GLUT/glut.h>

#include "matrix.h"

#include "dobj.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

using namespace std;

dobj d;

GLdouble yangle = 0.0; //the models all start reversed
GLdouble xangle = 0.0;
GLdouble zoom = 0.0;
bool isZoom = false;
bool showPoints = false;
bool clearColorToggle = false;
bool lightToggle = false;
bool isSmoothShaded = false;

GLdouble zeye = 0.0;

//used in the mouse move
int prevx, prevy;

//light params
GLfloat light_Ka[4] = {1.0f, 1.0f, 1.0f, 1.0};
GLfloat light_Kd[4] = {1.0f, 1.0f, 1.0f, 1.0};
GLfloat light_Ks[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat light_Ke = 1.0f;
 
GLfloat light_pos[4] = {0.0f, -100.0f, 100.0f, 1.0f};

/**
 * Display func
 */
void display()
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	
	glTranslated(0.0, 0.0, d.radius);
	
	glRotated(yangle, 0.0, 1.0, 0.0);
	glRotated(xangle, 1.0, 0.0, 0.0);
	
	if(showPoints)
	{
		d.drawVerts();
	}
	else
	{
		d.draw(isSmoothShaded);
	}
	
	glutSwapBuffers();
}

void calc_color(vert3 v, vert3 light, vec3 n, vert3 amb, vert3 dif)
{
	vec3 L = light - v;
	
	double dres = dot(L, n);
	double lambert = dres / (L.magnitude() * n.magnitude());
	
	lambert = lambert < 0? 0: lambert;
	
	cout << lambert << endl;
	
	cout << "Red = " << amb.x + dif.x*lambert << endl;
	cout << "Green = " << amb.y + dif.y*lambert << endl;
	cout << "Blue = " << amb.z + dif.z*lambert << endl;
}

/**
 * Keyboard callback
 */
void keyboard(unsigned char key, int x, int y)
{	
	GLdouble rot = d.radius*2.0;
	
	switch(key)
	{
		case 27: // ESC key
			exit(0);
			break;
		case 'w':
			xangle+=rot;
			break;
		case 'z':
			xangle-=rot;
			break;
		case 'a':
			yangle-=rot;
			break;
		case 's':
			yangle+=rot;
			break;
		case 'p':
			showPoints = showPoints? false:true;
			break;
		case 'b':
			if(clearColorToggle)
			{
				glClearColor(1.0, 1.0, 1.0, 0.0);
			}
			else
			{
				glClearColor(0.0, 0.0, 0.0, 0.0);
			}
			clearColorToggle = clearColorToggle? false:true;
			break;
		case 'l':
			if(lightToggle)
			{
				glEnable(GL_LIGHTING);
			}
			else
			{
				glDisable(GL_LIGHTING);
			}
			lightToggle = lightToggle? false:true;
			break;
		case 'm':
			isSmoothShaded = isSmoothShaded? false:true;
			break;
	}
}

/**
 * Mouse callback
 */
void mouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) //zoom
		{
			isZoom = true;
		}
		else //rotate
		{
			isZoom = false;
		}
		
		prevx=x;
		prevy=y;
	}
	else
	{
		isZoom = false;
	}
}

/**
 * Rotate the object on the screen
 */
void motion(int x, int y)
{
	if(!isZoom)
	{
		yangle+=prevx-x;
		xangle+=prevy-y;
	}
	else
	{
		zeye+=(prevy-y);
	}
	
	prevx = x;
	prevy = y;
}

/**
 * Put any 1-time initialization here
 */
void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glEnable(GL_NORMALIZE);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
	glLightf(GL_LIGHT0,GL_SHININESS, light_Ke);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-d.radius*1.3, d.radius*1.3, -d.radius, d.radius, 0.1, -d.radius*100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,0,0,1,0,0,1,0);
	
}

int main (int argc, char * argv[]) {	
	if(argc < 2)
	{
		cout << "Required argument filename.d not used." << endl;
	}
	
	d.loadObj(argv[1]);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow(".d Object Viewer");
	
	//callbacks
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	
	glutIdleFunc(display);
	
	init();
	
	vert3 v1 (0,0,1);
	vert3 v2 (0, 2*sqrt(2), -1/3);
	vert3 v3 (-6*sqrt(3), -sqrt(2)/3, -1/3);
	vert3 v4 (sqrt(6)/3, -sqrt(2)/3, -1/3);
	
	vec3 f1 = calc_face_norm(v1, v3, v2);
	vec3 f2 = calc_face_norm(v4, v2, v3);
	vec3 f3 = calc_face_norm(v1, v2, v4);
	vec3 f4 = calc_face_norm(v1, v4, v3);
	
	f1.normalize();
	f2.normalize();
	f3.normalize();
	f4.normalize();
	
	cout << "Face 1 norm "; f1.toString();
	cout << "Face 2 norm "; f2.toString();
	cout << "Face 3 norm "; f3.toString();
	cout << "Face 4 norm "; f4.toString();
	
	
	vec3 v1norm = f1 + f3 + f4;
	vec3 v2norm = f1 + f2 + f3;
	vec3 v3norm = f1 + f2 + f4;
	vec3 v4norm = f2 + f3 + f4;
	
	v1norm.normalize();
	v2norm.normalize();
	v3norm.normalize();
	v4norm.normalize();
	
	cout << "v1 norm " ; v1norm.toString();
	cout << "v2 norm " ; v2norm.toString();
	cout << "v3 norm " ; v3norm.toString();
	cout << "v4 norm " ; v4norm.toString();
	
	vert3 light (0,0,6);
	vert3 amb (.1, .1, .1);
	vert3 dif (.6, .3, .2);
	
	cout << "Vert 1 color" << endl;
	calc_color(v1, light, v1norm, amb, dif);
	
	cout << "Vert 2 color" << endl;
	calc_color(v2, light, v2norm, amb, dif);
	
	cout << "Vert 3 color" << endl;
	calc_color(v3, light, v3norm, amb, dif);
	
	cout << "Vert 4 color" << endl;
	calc_color(v4, light, v4norm, amb, dif);
	
	/*
	f1.normalize();
	f1.toString();
	
	f2.normalize();
	f2.toString();
	
	f3.normalize();
	f3.toString();
	
	f4.normalize();
	f4.toString();*/
	
	/*mat4D mat;
	vert3 eye (6, 0, 0);
	vert3 coi (0, 0, 0);
	make_world_to_eye_mat(eye, coi, mat);
	
	vert3 out;
	transform_pt3D(mat, v4, out);*/
	
	
	glutMainLoop();
	
    return 0;
}
