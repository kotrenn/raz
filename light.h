#ifndef __LIGHT_H
#define __LIGHT_H

#include "ray.h"

class Light
{
public:
	Light() {}
	virtual ~Light() {}
	
	virtual Ray get_ray(const vector3d &hit_point, float *dist) = 0;
	virtual float is_point() const = 0;
	virtual vector3d get_location() const = 0;
	void draw(GLenum light) const;
	
	vector3d get_ambient() const { return m_ambient; }
	vector3d get_diffuse() const { return m_diffuse; }
	vector3d get_specular() const { return m_specular; }
	void set_ambient(const vector3d &ambient) { m_ambient = ambient; }
	void set_diffuse(const vector3d &diffuse) { m_diffuse = diffuse; }
	void set_specular(const vector3d &specular) { m_specular = specular; }
private:
	vector3d m_ambient;
	vector3d m_diffuse;
	vector3d m_specular;
};

class PointLight : public Light
{
public:
	PointLight(const vector3d &loc);
	~PointLight();
	
	Ray get_ray(const vector3d &hit_point, float *dist);
	float is_point() const;
	vector3d get_location() const;
private:
	vector3d m_loc;
};

class DirectionLight : public Light
{
public:
	DirectionLight(const vector3d &dir);
	~DirectionLight();
	
	Ray get_ray(const vector3d &hit_point, float *dist);
	float is_point() const;
	vector3d get_location() const;
private:
	vector3d m_dir;
};

#endif
