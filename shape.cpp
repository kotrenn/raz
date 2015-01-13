#include "shape.h"

Shape::Shape()
	:m_material(NULL),
	 m_transform(new IdentityTransform())
{
}

Shape::~Shape()
{
	delete m_transform;
}

void Shape::draw() const
{
	if (m_material != NULL)
		m_material->draw();
	m_transform->draw();
}

float Shape::hits(const Ray &ray,
                  vector3d &normal,
                  Material **material)
{
	Ray inverted_ray = m_transform->invert(ray);
	float t = my_hits(inverted_ray, normal, material);
	if (t < 0)
		return -1;
	vector3d hit_point = inverted_ray.compute(t);
	hit_point = m_transform->apply(hit_point);
	t = (ray.get_loc() - hit_point).norm();
	normal = m_transform->apply_unit(normal);
	normal.normalize();
	return t;
}



Sphere::Sphere()
	:Shape(),
	 m_r(0),
	 m_quad(NULL)
{
}

Sphere::Sphere(float r)
	:Shape(),
	 m_r(r),
	 m_quad(NULL)
{
	m_quad = gluNewQuadric();
}

Sphere::Sphere(FILE *fp)
	:Shape(),
	 m_r(1.0),
	 m_quad(NULL)
{
	char att[80] = "";
	while (strcmp(att, "done") != 0)
	{
		fscanf(fp, "%s", att);
		if (strcmp(att, "radius") == 0)
			fscanf(fp, "%f", &m_r);
	}
	m_quad = gluNewQuadric();
}

Sphere::~Sphere()
{
	free(m_quad);
}

void Sphere::draw() const
{
	Shape::draw();
	
	gluSphere(m_quad, m_r, 20, 20);
}

float Sphere::my_hits(const Ray &ray,
                      vector3d &normal,
                      Material **material)
{
	vector3d v = ray.get_loc();
	vector3d d = ray.get_dir();
	float disc = (v * d) * (v * d) - (d * d) * (v * v - m_r * m_r);
	if (disc < 0)
		return -1;
	disc = sqrt(disc);
	float sub = ((-1.0 * v * d) - disc) / (d * d);
	float add = ((-1.0 * v * d) + disc) / (d * d);
	float lhs = min(sub, add);
	float rhs = max(sub, add);
	float t = lhs;
	if (t < 0)
		t = rhs;
	if (t < 0)
		return -1;
	vector3d hit = ray.compute(t);
	normal = hit;
	normal.normalize();
	*material = m_material;
	return t;
}



Plane::Plane()
	:Shape(),
	 m_point(),
	 m_normal()
{
}

Plane::Plane(const vector3d &point, const vector3d &normal)
	:Shape(),
	 m_point(point),
	 m_normal(normal)
{
	m_normal.normalize();
}

Plane::Plane(FILE *fp)
	:Shape(),
	 m_point(0, 0, 0),
	 m_normal(0, 0, 1)
{
	char att[80] = "";
	while (strcmp(att, "done") != 0)
	{
		fscanf(fp, "%s", att);
		
		float x, y, z;
		if (strcmp(att, "point") == 0)
		{
			fscanf(fp, "%f %f %f", &x, &y, &z);
			m_point = vector3d(x, y, z);
		}
		if (strcmp(att, "normal") == 0)
		{
			fscanf(fp, "%f %f %f", &x, &y, &z);
			m_normal = vector3d(x, y, z);
		}
	}
}

Plane::~Plane()
{
}

void Plane::draw() const
{
	Shape::draw();
	
	vector3d x(0, 0, 0);
	if ((((x - m_point) * m_normal) * m_normal - x).norm() < EPSILON)
		x = vector3d(1, 1, 1);
	x = vector3d(1, 1, 1);
	vector3d px = x - ((x - m_point) * m_normal) * m_normal;
	vector3d ux = px - m_point;
	ux.normalize();
	vector3d uy = m_normal % ux;
	float dist = 100.0;
	vector3d p00 = m_point - dist * ux - dist * uy;
	vector3d p01 = m_point - dist * ux + dist * uy;
	vector3d p10 = m_point + dist * ux - dist * uy;
	vector3d p11 = m_point + dist * ux + dist * uy;

	glBegin(GL_POLYGON);
	glVertex3f(p10[0], p10[1], p10[2]);
	glVertex3f(p11[0], p11[1], p11[2]);
	glVertex3f(p01[0], p01[1], p01[2]);
	glVertex3f(p00[0], p00[1], p00[2]);
	glEnd();
}

float Plane::my_hits(const Ray &ray,
                     vector3d &normal,
                     Material **material)
{
	vector3d v = ray.get_loc() - m_point;
	float top = -1.0 * v * m_normal;
	float bot = ray.get_dir() * m_normal;
	if (fabs(bot) < EPSILON)
		return -1;
	float t = top / bot;
	normal = m_normal;
	*material = m_material;
	return t;
}



Cone::Cone()
	:Shape(),
	 m_radius(0),
	 m_height(0)
{
}

Cone::Cone(float radius, float height)
	:Shape(),
	 m_radius(radius),
	 m_height(height)
{
}

Cone::Cone(FILE *fp)
	:Shape(),
	 m_radius(0),
	 m_height(0)
{
	char att[80] = "";
	while (strcmp(att, "done") != 0)
	{
		fscanf(fp, "%s", att);
		
		if (strcmp(att, "radius") == 0)
			fscanf(fp, "%f", &m_radius);
		if (strcmp(att, "height") == 0)
			fscanf(fp, "%f", &m_height);
	}
}

Cone::~Cone()
{
}

void Cone::draw() const
{
	Shape::draw();

	//glutSolidCone(m_radius, m_height, 20, 20);
}

float Cone::my_hits(const Ray &ray,
                    vector3d &normal,
                    Material **material)
{
	vector3d p = ray.get_loc();
	vector3d u = ray.get_dir();
	float r = m_radius;
	float h = m_height;

	float ux = u[0];
	float uy = u[1];
	float uz = u[2];
	float px = p[0];
	float py = p[1];
	float pz = p[2];
	
	float a = ux * ux + uy * uy - r * r * uz * uz / (h * h);
	float b = 2 * px * ux + 2 * py * uy - 2 * r * r * pz * uz / (h * h) + 2 * r * r * uz / h;
	float c = px * px + py * py - r * r - r * r * pz * pz / (h * h) + 2 * r * r * pz / h;
	
	float disc = b * b - 4 * a * c;
	if (disc < 0)
		return -1;
	disc = sqrt(disc);
	float sub = (-b - disc) / (2 * a);
	float add = (-b + disc) / (2 * a);
	float lhs = min(sub, add);
	float rhs = max(sub, add);
	float t = lhs;
	if (t < 0)
		t = rhs;
	else
	{
		vector3d hit = ray.compute(t);
		if (hit[2] < 0 || hit[2] > h)
			t = rhs;
	}
	if (t < 0)
		return -1;
	vector3d hit = ray.compute(t);

	if (hit[2] < 0 || hit[2] > h)
		return -1;

	float x = hit[0];
	float y = hit[1];
	float z = hit[2];
	normal = 2 * vector3d(x, y, r * r * (z - h) / (h * h));
	normal.normalize();
	*material = m_material;
	return t;
}



Cylinder::Cylinder()
	:Shape(),
	 m_radius(0),
	 m_height(0)
{
}

Cylinder::Cylinder(float radius, float height)
	:Shape(),
	 m_radius(radius),
	 m_height(height)
{
}

Cylinder::Cylinder(FILE *fp)
	:Shape(),
	 m_radius(0),
	 m_height(0)
{
	char att[80] = "";
	while (strcmp(att, "done") != 0)
	{
		fscanf(fp, "%s", att);
		
		if (strcmp(att, "radius") == 0)
			fscanf(fp, "%f", &m_radius);
		if (strcmp(att, "height") == 0)
			fscanf(fp, "%f", &m_height);
	}
}

Cylinder::~Cylinder()
{
}

void Cylinder::draw() const
{
	Shape::draw();

	int n = 20;
	float r = m_radius;
	float h = m_height;
	glBegin(GL_QUADS);
	for (int i = 0; i < n; ++i)
	{
		float t1 = (1.0 * i) / n;
		float t2 = (1.0 * (i + 1)) / n;
		t1 *= 2.0 * M_PI;
		t2 *= 2.0 * M_PI;
		glNormal3f(cos(t1), sin(t1), 0);
		glVertex3f(r * cos(t1), r * sin(t1), 0);
		glNormal3f(cos(t2), sin(t2), 0);
		glVertex3f(r * cos(t2), r * sin(t2), 0);
		glNormal3f(cos(t2), sin(t2), 0);
		glVertex3f(r * cos(t2), r * sin(t2), h);
		glNormal3f(cos(t1), sin(t1), 0);
		glVertex3f(r * cos(t1), r * sin(t1), h);
	}
	
	glEnd();
}

float Cylinder::my_hits(const Ray &ray,
                        vector3d &normal,
                        Material **material)
{
	vector3d p = ray.get_loc();
	vector3d u = ray.get_dir();
	float r = m_radius;
	float h = m_height;

	float ux = u[0];
	float uy = u[1];
	float uz = u[2];
	float px = p[0];
	float py = p[1];
	float pz = p[2];
	
	float a = ux * ux + uy * uy;
	float b = 2 * (px * ux + py * uy);
	float c = px * px + py * py - r * r;
	
	float disc = b * b - 4 * a * c;
	if (disc < 0)
		return -1;
	disc = sqrt(disc);
	float sub = (-b - disc) / (2 * a);
	float add = (-b + disc) / (2 * a);
	float lhs = min(sub, add);
	float rhs = max(sub, add);
	float t = lhs;
	if (t < 0)
		t = rhs;
	else
	{
		vector3d hit = ray.compute(t);
		if (hit[2] < 0 || hit[2] > h)
			t = rhs;
	}
	if (t < 0)
		return -1;
	vector3d hit = ray.compute(t);

	if (hit[2] < 0 || hit[2] > h)
		return -1;

	float x = hit[0];
	float y = hit[1];
	float z = hit[2];
	normal = 2 * vector3d(x, y, 0);
	normal.normalize();
	*material = m_material;
	return t;
}



Torus::Torus()
	:Shape(),
	 m_inner(0),
	 m_outer(0)
{
}

Torus::Torus(float inner, float outer)
	:Shape(),
	 m_inner(inner),
	 m_outer(outer)
{
}

Torus::Torus(FILE *fp)
	:Shape(),
	 m_inner(0),
	 m_outer(0)
{
	char att[80] = "";
	while (strcmp(att, "done") != 0)
	{
		fscanf(fp, "%s", att);
		
		if (strcmp(att, "inner") == 0)
			fscanf(fp, "%f", &m_inner);
		if (strcmp(att, "outer") == 0)
			fscanf(fp, "%f", &m_outer);
	}
}

Torus::~Torus()
{
}

void Torus::draw() const
{
	Shape::draw();

	//glutSolidTorus(m_inner, m_outer, 20, 20);
}

float Torus::my_hits(const Ray &ray,
                     vector3d &normal,
                     Material **material)
{
	vector3d s = ray.get_loc();
	vector3d v = ray.get_dir();
	float r1 = m_outer;
	float r2 = m_inner;

	float vx = v[0];
	float vy = v[1];
	float vz = v[2];
	float sx = s[0];
	float sy = s[1];
	float sz = s[2];

	float b = 4 * s * v;
	float c = 2 * (s * s + r1 * r1 - r2 * r2);
	c -= 4 * r1 * r1 * (1 - vz * vz);
	c += 4 * (s * v) * (s * v);
	float d = 8 * r1 * r1 * sz * vz;
	d += 4 * (s * v) * (s * s - r1 * r1 - r2 * r2);
	float e = sx * sx * sx * sx;
	e += sy * sy * sy * sy;
	e += sz * sz * sz * sz;
	e += (r1 * r1 - r2 * r2) * (r1 * r1 - r2 * r2);
	e += 2 * sx * sx * sy * sy;
	e += 2 * sz * sz * (r1 * r1 - r2 * r2);
	e += 2 * (sx * sx + sy * sy) * (sz * sz - r1 * r1 - r2 * r2);

	set<float> roots = Math::quartic(b, c, d, e);
	set<float>::iterator it;
	float t = -1;
	for (it = roots.begin(); it != roots.end(); it++)
		if (*it > 0 && t < 0)
			t = *it;
	if (t < 0)
		return -1;
	
	vector3d hit = ray.compute(t);

	float x = hit[0];
	float y = hit[1];
	float z = hit[2];
	//float mid = x * x + y * y + z * z;
	float mid = hit * hit;
	normal = vector3d(4 * x * (mid - r1 * r1 - r2 * r2),
	                  4 * y * (mid - r1 * r1 - r2 * r2),
	                  4 * z * (r1 * r1 - r2 * r2 + mid));
	normal.normalize();
	*material = m_material;
	return t;
}
