#ifndef __MATERIAL_H
#define __MATERIAL_H

#include "vector3d.h"

class Material
{
public:
	Material();
	Material(FILE *fp);
	~Material();

	vector3d get_ambient() const { return m_ambient; }
	vector3d get_diffuse() const { return m_diffuse; }
	vector3d get_specular() const { return m_specular; }
	float get_shininess() const { return m_shininess; }
	void set_ambient(const vector3d &ambient) { m_ambient = ambient; }
	void set_diffuse(const vector3d &diffuse) { m_diffuse = diffuse; }
	void set_specular(const vector3d &specular) { m_specular = specular; }
	void set_shininess(const float &shininess) { m_shininess = shininess; }

	void draw() const;
private:
	vector3d m_ambient;
	vector3d m_diffuse;
	vector3d m_specular;
	float m_shininess;
};

#endif
