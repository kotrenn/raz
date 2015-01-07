#include "light.h"

void Light::draw(GLenum light) const
{
	float point = is_point();
	vector3d loc = get_location();
	GLfloat location[] = { loc[0], loc[1], loc[2], point };
	GLfloat ambient[] = { m_ambient[0],
	                      m_ambient[1],
	                      m_ambient[2],
	                      1.0 };
	GLfloat diffuse[] = { m_diffuse[0],
	                      m_diffuse[1],
	                      m_diffuse[2],
	                      1.0 };
	GLfloat specular[] = { m_specular[0],
	                       m_specular[1],
	                       m_specular[2],
	                       1.0 };
	glLightfv(light, GL_POSITION, location);
	glLightfv(light, GL_AMBIENT, ambient);
	glLightfv(light, GL_DIFFUSE, diffuse);
	glLightfv(light, GL_SPECULAR, specular);
}




PointLight::PointLight(const vector3d &loc)
	:Light(),
	 m_loc(loc)
{
}

PointLight::~PointLight()
{
}

Ray PointLight::get_ray(const vector3d &loc, float *dist)
{
	vector3d v = m_loc - loc;
	*dist = v.norm();
	v /= *dist;
	return Ray(loc, v);
}

float PointLight::is_point() const
{
	return 1.0;
}

vector3d PointLight::get_location() const
{
	return m_loc;
}



DirectionLight::DirectionLight(const vector3d &dir)
	:Light(),
	 m_dir(dir)
{
	m_dir.normalize();
}

DirectionLight::~DirectionLight()
{
}

Ray DirectionLight::get_ray(const vector3d &loc, float *dist)
{
	*dist = INFINITY;
	return Ray(loc, -1.0 * m_dir);
}

float DirectionLight::is_point() const
{
	return 0.0;
}

vector3d DirectionLight::get_location() const
{
	return m_dir;
}
