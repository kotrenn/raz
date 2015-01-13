#ifndef __COMMON_H
#define __COMMON_H

#ifdef __linux__
#elif _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>

#define GLEW_STATIC

#define NO_SDL_GLEXT
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

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

float clamp(float x, float a, float b);
#define signum(X) (((X) > 0) - ((X) < 0))

int randint(int a, int b);
float randdbl(float a, float b);
float random_01();

/* generate continuous approximation
   of geometric distribution of p
*/
float geometric(float p);

#endif
