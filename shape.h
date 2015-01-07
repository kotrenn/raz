#ifndef __SHAPE_H
#define __SHAPE_H

#include "ray.h"
#include "material.h"
#include "transform.h"

class Shape
{
public:
	Shape();
	virtual ~Shape();

	virtual void draw() const;
	
	float hits(const Ray &ray,
	           vector3d &normal,
	           Material **material);
	void set_material(Material *material) { m_material = material; }
	Material *get_material() const { return m_material; }

	void set_transform(Transform *transform) { m_transform = transform; }
	Transform *get_transform() const { return m_transform; }
protected:
	virtual float my_hits(const Ray &ray,
	                      vector3d &normal,
	                      Material **material) = 0;
	Material *m_material;
	Transform *m_transform;
};

class Sphere : public Shape
{
public:
	Sphere();
	Sphere(float r);
	Sphere(FILE *fp);
	~Sphere();

	void draw() const;
private:
	float my_hits(const Ray &ray,
	              vector3d &normal,
	              Material **material);
	
	float m_r;
	GLUquadric *m_quad;
};

class Plane : public Shape
{
public:
	Plane();
	Plane(const vector3d &point, const vector3d &normal);
	Plane(FILE *fp);
	~Plane();

	void draw() const;
private:
	float my_hits(const Ray &ray,
	              vector3d &normal,
	              Material **material);
	
	vector3d m_point;
	vector3d m_normal;
};

class Cone : public Shape
{
public:
	Cone();
	Cone(float radius, float height);
	Cone(FILE *fp);
	~Cone();

	void draw() const;
private:
	float my_hits(const Ray &ray,
	              vector3d &normal,
	              Material **material);

	float m_radius;
	float m_height;
};

class Cylinder : public Shape
{
public:
	Cylinder();
	Cylinder(float radius, float height);
	Cylinder(FILE *fp);
	~Cylinder();

	void draw() const;
private:
	float my_hits(const Ray &ray,
	              vector3d &normal,
	              Material **material);

	float m_radius;
	float m_height;
};

class Torus : public Shape
{
public:
	Torus();
	Torus(float inner, float outer);
	Torus(FILE *fp);
	~Torus();

	void draw() const;
private:
	float my_hits(const Ray &ray,
	              vector3d &normal,
	              Material **material);

	float m_inner;
	float m_outer;
};

#endif
