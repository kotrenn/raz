#ifndef __SCENE_H
#define __SCENE_H

#include "shape.h"
#include "light.h"
#include "matlib.h"

class Scene
{
public:
	Scene();
	~Scene();

	void setup();
	void draw();

	vector3d shoot(const Ray &ray, Shape *prev, int depth);
	float hits(const Ray &ray,
	           Shape *prev,
	           Shape **ret,
	           vector3d &normal,
	           Material **material,
	           float max_dist = INFINITY);
private:
	MatLib *m_matlib;
	list<Shape *> m_shapes;
	list<Light *> m_lights;
};

#endif
