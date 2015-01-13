#include "scene.h"
#include "model.h"

Scene::Scene()
	:m_matlib(new MatLib()),
	 m_shapes(),
	 m_lights()
{
	/*
	FILE *material_fp;
	if ((material_fp = fopen("material.txt", "r")) == NULL)
	{
	}
	else
	{
		char type[80];
		char name[80];
		while (fscanf(material_fp, "%s %s", type, name) == 2)
		{
			Material *mat = new Material(material_fp);
			m_materials[name] = mat;
		}
		
		fclose(material_fp);
	}
	*/

	m_matlib = new MatLib();
	m_matlib->open("test.mtl");
	
	FILE *world_fp;
	if ((world_fp = fopen("world.txt", "r")) == NULL)
	{
		/* default world here */
	}
	else
	{
		char buf[80];
		while (fscanf(world_fp, "%s", buf) > 0)
		{
			if (strcmp(buf, "mtllib") == 0)
			{
				char name[80];
				fscanf(world_fp, "%s", name);
				m_matlib->open(name);
			}
			else if (strcmp(buf, "object") == 0)
			{
				char name[80];
				fscanf(world_fp, "%s", name);
				char shape_decl[80];
				fscanf(world_fp, "%s", shape_decl);
				if (strcmp(shape_decl, "shape") != 0)
				{
					printf("Error: non-shape attribute %s\n", shape_decl);
					exit(1);
				}
				
				char type[80];
				fscanf(world_fp, "%s", type);
				
				Shape *shape = NULL;
				if (strcmp(type, "sphere") == 0)
					shape = new Sphere(world_fp);
				else if (strcmp(type, "plane") == 0)
					shape = new Plane(world_fp);
				else if (strcmp(type, "cone") == 0)
					shape = new Cone(world_fp);
				else if (strcmp(type, "cylinder") == 0)
					shape = new Cylinder(world_fp);
				else if (strcmp(type, "torus") == 0)
					shape = new Torus(world_fp);
				else if (strcmp(type, "model") == 0)
					shape = new Model(world_fp, m_matlib);
				else
				{
					printf("Unknown shape %s\n", type);
					exit(1);
				}

				char att[80] = "";
				while (strcmp(att, "done") != 0)
				{
					Transform *transform = shape->get_transform();
					fscanf(world_fp, "%s", att);
					if (strcmp(att, "material") == 0)
					{
						char mat[80];
						fscanf(world_fp, "%s", mat);
						shape->set_material(m_matlib->get(mat));
					}
					else if (strcmp(att, "translate") == 0)
					{
						float x, y, z;
						fscanf(world_fp, "%f %f %f", &x, &y, &z);
						vector3d delta(x, y, z);
						transform = new TranslateTransform(transform,
						                                   delta);
					}
					else if (strcmp(att, "scale") == 0)
					{
						float x, y, z;
						fscanf(world_fp, "%f %f %f", &x, &y, &z);
						vector3d scale(x, y, z);
						transform = new ScaleTransform(transform,
						                               scale);
					}
					else if (strcmp(att, "rotate") == 0)
					{
						float theta;
						fscanf(world_fp, "%f", &theta);
						float x, y, z;
						fscanf(world_fp, "%f %f %f", &x, &y, &z);
						vector3d axis(x, y, z);
						transform = new RotateTransform(transform,
						                                theta,
						                                axis);
					}
					else if (strcmp(att, "done") != 0)
						printf("Unknown attribute %s\n", att);

					shape->set_transform(transform);
				} // done reading object

				m_shapes.push_back(shape);
			}
		}
		fclose(world_fp);
	}
	
	FILE *lights_fp;
	if ((lights_fp = fopen("lights.txt", "r")) == NULL)
	{
		printf("Default lights\n");
		/* default light here */
	}
	else
	{
		char type[80];
		while (fscanf(lights_fp, "%s", type) > 0)
		{
			char att[80] = "";
			float x, y, z;
			Light *light = NULL;
			while (strcmp(att, "done") != 0)
			{
				fscanf(lights_fp, "%s", att);
				if (strcmp(att, "done") != 0)
					fscanf(lights_fp, "%f %f %f", &x, &y, &z);
				vector3d v(x, y, z);
				if (strcmp(att, "loc") == 0)
				{
					if (strcmp(type, "pointlight") == 0)
						light = new PointLight(v);
					else if (strcmp(type, "directionlight") == 0)
						light = new DirectionLight(v);
					m_lights.push_back(light);
				}
				else if (strcmp(att, "ambient") == 0)
					light->set_ambient(v);
				else if (strcmp(att, "diffuse") == 0)
					light->set_diffuse(v);
				else if (strcmp(att, "specular") == 0)
					light->set_specular(v);
			}
		}
		fclose(lights_fp);
	}
}

Scene::~Scene()
{
	//map<string, Material *>::iterator mi;
	list<Shape *>::iterator si;
	list<Light *>::iterator li;
	//for (mi = m_materials.begin(); mi != m_materials.end(); mi++)
//		delete mi->second;
	delete m_matlib;
	for (si = m_shapes.begin(); si != m_shapes.end(); si++)
		delete *si;
	for (li = m_lights.begin(); li != m_lights.end(); li++)
		delete *li;
}

void Scene::setup()
{
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	for (unsigned int i = 0; i < m_lights.size(); ++i)
		glEnable(GL_LIGHT0 + i + 1);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	glClearDepth(1.0);
	glClearColor(0, 0, 0, 0);
}

float knot_x(float u, float v)
{
	return cos(u) + 3 * cos(3 * u) + 0.25 * cos(v) * cos(3 * u);
}

float knot_y(float u, float v)
{
	return sin(u) + 3 * sin(3 * u) + 0.25 * cos(v) * sin(3 * u);
}

float knot_z(float u, float v)
{
	//return sin(2 * u) + 0.25 * sin(v);
	float z = sin(2 * u) + 0.25 * sin(v);
	return 3.0 * z;
}

void Scene::draw()
{
	list<Shape *>::iterator si;
	for (si = m_shapes.begin(); si != m_shapes.end(); si++)
	{
		glPushMatrix();
		Shape *shape = *si;
		shape->draw();
		glPopMatrix();
	}

	/*
	glPushMatrix();
	glutSolidTeapot(2.0);
	glPopMatrix();

	glPushMatrix();
	int n = 100;
	int m = 100;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			float u0 = (1.0 * i) / n;
			float v0 = (1.0 * j) / m;
			float u1 = (1.0 * (i + 1)) / n;
			float v1 = (1.0 * (j + 1)) / m;

			float s0 = u0;
			float t0 = v0 / 10;
			float s1 = u1;
			float t1 = v1 / 10;

			u0 = 2.0 * M_PI * u0 - M_PI;
			v0 = 2.0 * M_PI * v0 - M_PI;
			u1 = 2.0 * M_PI * u1 - M_PI;
			v1 = 2.0 * M_PI * v1 - M_PI;

			glBegin(GL_POLYGON);
			glTexCoord2f(s0, t0);
			glVertex3f(knot_x(u0, v0), knot_y(u0, v0), knot_z(u0, v0));
			glTexCoord2f(s1, t0);
			glVertex3f(knot_x(u1, v0), knot_y(u1, v0), knot_z(u1, v0));
			glTexCoord2f(s1, t1);
			glVertex3f(knot_x(u1, v1), knot_y(u1, v1), knot_z(u1, v1));
			glTexCoord2f(s0, t1);
			glVertex3f(knot_x(u0, v1), knot_y(u0, v1), knot_z(u0, v1));
			glEnd();
		}
	}
	glPopMatrix();
	*/
	
	list<Light *>::iterator li;
	int i = 1;
	for (li = m_lights.begin(); li != m_lights.end(); li++)
	{
		if (i < GL_MAX_LIGHTS)
		{
			Light *light = *li;
			light->draw(GL_LIGHT0 + i);
		}
		i++;
	}
}

vector3d Scene::shoot(const Ray &ray, Shape *prev, int depth)
{
	vector3d background(0, 0, 0);

	if (depth < 0)
		return background;
	
	Shape *closest;
	vector3d normal;
	Material *material;
	float t = hits(ray, prev, &closest, normal, &material);
	
	if (t <= 0)
		return background; // emptiness

	vector3d hit_point = ray.compute(t);

	vector3d ret_color(0, 0, 0);
	vector3d K_a = material->get_ambient();
	vector3d K_d = material->get_diffuse();
	vector3d K_s = material->get_specular();
	
	/* visit all lights */
	Shape *block;
	list<Light *>::iterator lt;
	for (lt = m_lights.begin(); lt != m_lights.end(); lt++)
	{
		Light *light = *lt;
		float dist;
		Ray light_ray = light->get_ray(hit_point, &dist);
		vector3d tmp_normal;
		Material *tmp_mat;
		
		vector3d I_a = light->get_ambient();
		ret_color += I_a ^ K_a;

		if (hits(light_ray, closest, &block,
		         tmp_normal, &tmp_mat, dist) < 0 &&
		    light_ray.get_dir() * normal > 0)
		{
			vector3d L = light_ray.get_dir();
			
			/* ambient light */
			/* diffuse light */
			vector3d I_d = light->get_diffuse();
			ret_color += (I_d ^ K_d) * (normal * L);

			/* specular light */
			vector3d I_s = light->get_diffuse();
			float n = material->get_shininess();
			vector3d V = -1.0 * ray.get_dir();
			vector3d H = L + V / (L.norm() + V.norm());
			H.normalize();
			ret_color += (I_s ^ K_s) * pow(H * normal, n);
		}
		
		if (K_s.norm() > EPSILON)
		{
			vector3d V = -1.0 * ray.get_dir();
			vector3d R = 2.0 * normal * (normal * V) - V;
			Ray reflect(hit_point, R);
			vector3d I_r = shoot(reflect, closest, depth - 1);
			ret_color += K_s ^ I_r;
		}
	}
	
	return ret_color;
}

float Scene::hits(const Ray &ray,
                  Shape *prev,
                  Shape **ret,
                  vector3d &normal,
                  Material **material,
                  float max_dist)
{
	bool hit = false;
	Shape *closest = NULL;
	float min_t = INFINITY;
	
	list<Shape *>::iterator it;
	for (it = m_shapes.begin(); it != m_shapes.end(); it++)
	{
		Shape *cur = *it;
		if (cur != prev)
		{
			vector3d cur_normal;
			Material *cur_material;
			float t = cur->hits(ray, cur_normal, &cur_material);
			if (t > 0 && t < max_dist)
			{
				hit = true;
				if (t < min_t)
				{
					min_t = t;
					closest = cur;
					normal = cur_normal;
					*material = cur_material;
				}
			}
		}
	}

	if (!hit)
		return -1;

	*ret = closest;
	return min_t;
}
