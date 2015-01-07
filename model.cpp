#include "model.h"

Face::Face()
	:m_vertex(),
	 m_normal(),
	 m_mtl(NULL)
{
}

void Face::add_v(vector3d *v)
{
	m_vertex.push_back(v);
}

void Face::add_n(vector3d *n)
{
	m_normal.push_back(n);
}

vector3d Face::get_n(int v) const
{
	if (m_normal[v] == NULL)
	{
		vector3d p0 = *(m_vertex[0]);
		vector3d p1 = *(m_vertex[1]);
		vector3d p2 = *(m_vertex[2]);
		return (p0 - p1) % (p0 - p2);
	}
	else
		return *(m_normal[v]);
}

float Face::hits(const Ray &ray, vector3d &normal)
{
	for (unsigned int i = 1; i < m_vertex.size() - 1; ++i)
	{
		float t = tri_hits(ray, normal, 0, i, i + 1);
		if (t >= 0)
			return t;
	}
	return -1;
}

float Face::tri_hits(const Ray &ray,
                     vector3d &normal,
                     int v0, int v1, int v2)
{
	vector3d p0 = *(m_vertex[v0]);
	vector3d p1 = *(m_vertex[v1]);
	vector3d p2 = *(m_vertex[v2]);
	vector3d n = (p0 - p1) % (p0 - p2);
	vector3d b = p0 - ray.get_loc();
	float adotcXd = (ray.get_dir() * n);
	float u, v, t;
	if (adotcXd > 0)
	{
		u = ray.get_dir() * (b % (p0 - p2));
		if (u < 0.0f || u > adotcXd)
			return -1;

		v = ray.get_dir() * ((p0 - p1) % b);
		if (v < 0.0f || u + v > adotcXd)
			return -1;
	}
	else
	{
		u = ray.get_dir() * (b % (p0 - p2));
		if (u > 0.0f || u < adotcXd)
			return -1;

		v = ray.get_dir() * ((p0 - p1) % b);
		if (v > 0.0f || u + v < adotcXd)
			return -1;
	}

	adotcXd = 1.0f / adotcXd;

	t = (b * n) * adotcXd;
	if (t < 0)
		return -1;

	u = u * adotcXd;
	v = v * adotcXd;

	normal = n / n.norm();

	vector3d n0 = m_normal[0] == NULL? n : *(m_normal[0]);
	vector3d n1 = m_normal[1] == NULL? n : *(m_normal[1]);
	vector3d n2 = m_normal[2] == NULL? n : *(m_normal[2]);
	n0.normalize();
	n1.normalize();
	n2.normalize();

	normal = u * n1 + v * n2 + (1 - u - v) * n0;
	//normal = n / n.norm();
	return t;
}



Model::Model(const char *filename, MatLib *matlib)
	:Shape(),
	 m_matlib(matlib),
	 m_v_count(0), m_f_count(0),
	 m_n_count(0), m_t_count(0),
	 m_vertex(NULL),
	 m_normal(NULL),
	 m_face(NULL),
	 m_cur_v(0), m_cur_f(0),
	 m_cur_n(0), m_cur_t(0),
	 m_cur_mtl("")
{
	load(filename);
}

Model::Model(FILE *fp, MatLib *matlib)
	:Shape(),
	 m_matlib(matlib),
	 m_v_count(0), m_f_count(0),
	 m_n_count(0), m_t_count(0),
	 m_vertex(NULL),
	 m_normal(NULL),
	 m_face(NULL),
	 m_cur_v(0), m_cur_f(0),
	 m_cur_n(0), m_cur_t(0),
	 m_cur_mtl("")
{
	bool found = false;
	
	char att[80] = "";
	while (strcmp(att, "done") != 0)
	{
		fscanf(fp, "%s", att);
		if (strcmp(att, "file") == 0)
		{
			char filename[80];
			fscanf(fp, "%s", filename);
			if (found)
				printf("Error: loading repeat model file %s\n", filename);
			else
				load(filename);
			found = true;
		}
	}
}

Model::~Model()
{
	free(m_vertex);
	free(m_normal);
	free(m_face);
}

/* Error message when something goes wrong with loading */
void Model::corrupt(const char *filename)
{
	fprintf(stderr, "Corrupt file: \"%s\"\n", filename);
	exit(1);
}

/* Skips through whitespace and comments */
int Model::whitespace(FILE *in)
{
	int ch;
	while ((isspace(ch = fgetc(in)) || ch == '#') && ch != EOF)
		if (ch == '#')
			while ((ch = fgetc(in)) != '\n' && ch != EOF) {}
	return ch;
}

/* Read a character with corrupt */
int Model::readc(FILE *in, const char *filename)
{
	int c = fgetc(in);
	if (c == EOF) corrupt(filename);
	return c;
}

/* Read to the end of the line */
int Model::get_line(FILE *in)
{
	int ch;
	while ((ch = fgetc(in)) != '\n' && ch != EOF) {}
	return ch;
}

/* Read in a vertex */
void Model::get_vertex(FILE *in, const char *filename)
{
	float x, y, z;
	if (fscanf(in, "%f %f %f", &x, &y, &z) < 3)
		corrupt(filename);
	float s = 1;
	x *= s;
	y *= s;
	z *= s;
	m_vertex[++m_cur_v] = vector3d(x, y, z);
}

/* Read in a face */
void Model::get_face(FILE *in, const char *filename)
{
	Face &f = m_face[++m_cur_f] = Face();
	f.set_mtl(m_matlib->get(m_cur_mtl));
	int v, n, ch = '\0';
	while (ch != '\n')
	{
		ch = whitespace(in);
		v = ch - '0';
		while (!isspace(ch = readc(in, filename)) && ch != '/')
			v = 10 * v + ch - '0';
		f.add_v(&(m_vertex[v]));
		if (ch == '/')
			while (!isspace(ch = readc(in, filename)) && ch != '/') {}
		if (ch == '/')
		{
			n = 0;
			while (!isspace(ch = readc(in, filename)))
				n = 10 * n + ch - '0';
			f.add_n(&(m_normal[n]));
		}
		else
			f.add_n(NULL);
	}
}

/* Would read in a normal, could later be defined */
void Model::get_normal(FILE *in, const char *filename)
{
	float x, y, z;
	if (fscanf(in, "%f %f %f", &x, &y, &z) < 3)
		corrupt(filename);
	m_normal[++m_cur_n] = vector3d(x, y, z);
	m_normal[m_cur_n].normalize();
}

/* Would read in a texture coordinate, could later be defined */
void Model::get_texture(FILE *in, const char *filename)
{
}

/* Would read in a material, could later be defined */
void Model::get_material(FILE *in, const char *filename)
{
	string mtl("");
	int ch;
	while (!isspace(ch = readc(in, filename)))
		mtl += ch;
	m_matlib->open(mtl.c_str());
}

void Model::get_mtl(FILE *in, const char *filename)
{
	m_cur_mtl = "";
	int ch;
	while (!isspace(ch = readc(in, filename)))
		m_cur_mtl += ch;
}

/* Load in the object from the file in filename */
void Model::load(const char *filename)
{
	FILE *in;
	int ch;
	if ((in = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Error: could not read model \"%s\"\n", filename);
		exit(1);
	}

	char buf[10];
	int n = 0;

	/* First pass just counts to see how much
	   memory should be allocated */
	m_v_count = m_f_count = m_n_count = m_t_count = 0;
	ch = whitespace(in);
	while (ch != EOF)
	{
		buf[n = 0] = ch;
		while (!isspace(ch = readc(in, filename)))
			buf[++n] = ch;
		buf[++n] = '\0';
		if (strcmp(buf, "v") == 0) { m_v_count++; ch = get_line(in); }
		else if (strcmp(buf, "f") == 0) { m_f_count++; ch = get_line(in); }
		else if (strcmp(buf, "vn") == 0) { m_n_count++; ch = get_line(in); }
		else if (strcmp(buf, "vt") == 0) { m_t_count++; ch = get_line(in); }
		else if (strcmp(buf, "g") == 0) { ch = get_line(in); }
		else if (strcmp(buf, "usemtl") == 0) { ch = get_line(in); }
		else ch = get_line(in);
		ch = whitespace(in);
	}
	m_vertex = (vector3d *)malloc(sizeof(vector3d) * (m_v_count + 1));
	m_normal = (vector3d *)malloc(sizeof(vector3d) * (m_n_count + 1));
	m_face = (Face *)malloc(sizeof(Face) * (m_f_count + 1));
	rewind(in);

	/* Second pass actually reads in the data */
	m_cur_v = m_cur_f = m_cur_n = m_cur_t = 0;
	ch = whitespace(in);
	while (ch != EOF)
	{
		buf[n = 0] = ch;
		while (!isspace(ch = readc(in, filename)))
			buf[++n] = ch;
		buf[++n] = '\0';
		if (strcmp(buf, "v") == 0) get_vertex(in, filename);
		else if (strcmp(buf, "f") == 0) get_face(in, filename);
		else if (strcmp(buf, "vn") == 0) get_normal(in, filename);
		else if (strcmp(buf, "vt") == 0) ch = get_line(in);
		else if (strcmp(buf, "g") == 0) ch = get_line(in);
		else if (strcmp(buf, "mtllib") == 0) get_material(in, filename);
		else if (strcmp(buf, "usemtl") == 0) get_mtl(in, filename);
		else ch = get_line(in);
		if (ch != EOF)
			ch = whitespace(in);
	}

	fclose(in);
}

/* Draw the model */
void Model::draw() const
{
	Shape::draw();
	
	glColor3f(1.0, 0.0, 1.0);

	glLineWidth(1);

	int cur_f = 0;
	Material *prev_mat = NULL;

	/* Loop over the faces and their vertices */
	while (cur_f < m_f_count)
	{
		Face &f = m_face[++cur_f];
		Material *cur_mat = f.get_mtl();
		if (cur_mat != prev_mat)
			cur_mat->draw();
		prev_mat = cur_mat;
		
		glBegin(GL_POLYGON);
		int cur_v = 0;
		while (cur_v < f.get_v_count())
		{
			vector3d n = f.get_n(cur_v);
			glNormal3f(n[0], n[1], n[2]);
			vector3d v = f.get_v(cur_v);
			glVertex3f(v[0], v[1], v[2]);
			cur_v++;
		}
		glEnd();
	}
}

float Model::my_hits(const Ray &ray,
                     vector3d &normal,
                     Material **material)
{
	bool hit = false;
	float min_t = INFINITY;
	//Face *closest = NULL;
	
	m_cur_f = 0;
	while (m_cur_f < m_f_count)
	{
		Face &f = m_face[++m_cur_f];
		vector3d f_n;
		float t = f.hits(ray, f_n);
		if (t < min_t && t > 0)
		{
			hit = true;
			normal = f_n;
			//closest = &(m_face[m_cur_f]);
			min_t = t;
			*material = f.get_mtl();
		}
	}

	if (!hit)
		return -1;

	return min_t;
}
