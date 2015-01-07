#include "matlib.h"

MatLib::MatLib()
	:m_materials(),
	 m_cur_name(""),
	 m_cur(NULL),
	 m_files()
{
	Material *white = new Material();
	white->set_ambient(vector3d(0.2, 0.2, 0.2));
	white->set_diffuse(vector3d(0.8, 0.8, 0.8));
	white->set_specular(vector3d(0, 0, 0));
	white->set_shininess(1);
	m_materials["white"] = white;
}

MatLib::~MatLib()
{
	map<string, Material *>::iterator it;
	for (it = m_materials.begin(); it != m_materials.end(); it++)
		delete it->second;
}

void MatLib::corrupt(const char *filename)
{
	fprintf(stderr, "Corrupt file: \"%s\"\n", filename);
	exit(1);
}

int MatLib::whitespace(FILE *in)
{
	int ch;
	while ((isspace(ch = fgetc(in)) || ch == '#') && ch != EOF)
		if (ch == '#')
			while ((ch = fgetc(in)) != '\n' && ch != EOF);
	return ch;
}

int MatLib::readc(FILE *in, const char *filename)
{
	int c = fgetc(in);
	if (c == EOF) corrupt(filename);
	return c;
}

int MatLib::get_line(FILE *in)
{
	int ch;
	while ((ch = fgetc(in)) != '\n' && ch != EOF);
	return ch;
}

void MatLib::get_mtl(FILE *in, const char *filename)
{
	if (m_cur != NULL)
		m_materials[m_cur_name] = m_cur;

	int ch;
	m_cur_name = "";
	while (!isspace(ch = readc(in, filename)))
		m_cur_name += ch;
	m_cur = new Material();
}

void MatLib::get_att(FILE *in, const char *filename, const string &att)
{
	float r, g, b;
	if (fscanf(in, "%f %f %f", &r, &g, &b) < 3)
		corrupt(filename);
	vector3d v(r, g, b);
	if (att == "a")
		m_cur->set_ambient(v);
	if (att == "d")
		m_cur->set_diffuse(v);
	if (att == "s")
		m_cur->set_specular(v);
}

void MatLib::get_shininess(FILE *in, const char *filename)
{
	float n;
	if (fscanf(in, "%f", &n) < 1)
		corrupt(filename);
	m_cur->set_shininess(n);
}
		
void MatLib::open(const char *filename)
{
	/* avoid opening a file twice */
	if (m_files.find(filename) != m_files.end())
		return;

	m_files.insert(filename);
	
	FILE *in;
	int ch;
	if ((in = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Error: could not read \"%s\"\n", filename);
		exit(1);
	}

	char buf[10];
	int n = 0;

	ch = whitespace(in);
	while (ch != EOF)
	{
		buf[n = 0] = ch;
		while (!isspace(ch = readc(in, filename)))
			buf[++n] = ch;
		buf[++n] = '\0';
		if (strcmp(buf, "newmtl") == 0) get_mtl(in, filename);
		else if (strcmp(buf, "Ka") == 0) get_att(in, filename, "a");
		else if (strcmp(buf, "Kd") == 0) get_att(in, filename, "d");
		else if (strcmp(buf, "Ks") == 0) get_att(in, filename, "s");
		else if (strcmp(buf, "Ns") == 0) get_shininess(in, filename);
		else ch = get_line(in);
		if (ch != EOF)
			ch = whitespace(in);
	}

	if (m_cur != NULL)
		m_materials[m_cur_name] = m_cur;
	m_cur = NULL;
}

Material *MatLib::get(const string &name)
{
	if (m_materials.find(name) == m_materials.end())
		return m_materials["white"];
	return m_materials[name];
}
