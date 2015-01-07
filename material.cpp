#include "material.h"

Material::Material()
	:m_ambient(),
	 m_diffuse(),
	 m_specular(),
	 m_shininess(0)
{
}

Material::Material(FILE *fp)
	:m_ambient(),
	 m_diffuse(),
	 m_specular(),
	 m_shininess(5)
{
	float r, g, b;
	char buf[80] = "";
	while (strcmp(buf, "done") != 0)
	{
		fscanf(fp, "%s", buf);
		if (strcmp(buf, "ambient") == 0)
		{
			fscanf(fp, "%f %f %f", &r, &g, &b);
			m_ambient = vector3d(r, g, b);
		}
		if (strcmp(buf, "diffuse") == 0)
		{
			fscanf(fp, "%f %f %f", &r, &g, &b);
			m_diffuse = vector3d(r, g, b);
		}
		if (strcmp(buf, "specular") == 0)
		{
			fscanf(fp, "%f %f %f", &r, &g, &b);
			m_specular = vector3d(r, g, b);
		}
		if (strcmp(buf, "shininess") == 0)
		{
			fscanf(fp, "%f", &r);
			m_shininess = r;
		}
	}
}

Material::~Material()
{
}

void Material::draw() const
{
	GLfloat mat_ambient[] = { m_ambient[0],
	                          m_ambient[1],
	                          m_ambient[2],
	                          1.0 };
	GLfloat mat_diffuse[] = { m_diffuse[0],
	                          m_diffuse[1],
	                          m_diffuse[2],
	                          1.0 };
	GLfloat mat_specular[] = { m_specular[0],
	                           m_specular[1],
	                           m_specular[2],
	                           1.0 };
	GLfloat mat_shininess[] = { m_shininess };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}	
