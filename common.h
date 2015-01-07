#ifndef __COMMON_H
#define __COMMON_H

#include <windows.h>

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define GLEW_STATIC

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include <list>
#include <map>
#include <string>
#include <vector>
#include <set>

#define EPSILON 0.000001

using namespace std;

class Math
{
public:
	static float cube_root(float x);
	static set<float> quadratic(float a, float b, float c);
	static set<float> cubic(float a, float b, float c);
	static set<float> quartic(float a, float b, float c, float d);
};

#endif
