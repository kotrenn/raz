#ifndef __MODEL_H
#define __MODEL_H

#include "shape.h"
#include "matlib.h"

class Face
{
public:
	Face();

	int get_v_count() const { return m_vertex.size(); }
	void add_v(vector3d *v);
	void add_n(vector3d *n);
	vector3d get_v(int v) const { return *(m_vertex[v]); }
	vector3d get_n(int v) const;
	void set_mtl(Material *mtl) { m_mtl = mtl; }
	Material *get_mtl() const { return m_mtl; }

	float hits(const Ray &ray, vector3d &normal);
private:
	float tri_hits(const Ray &ray,
	               vector3d &normal,
	               int v0, int v1, int v2);
	vector<vector3d *> m_vertex;
	vector<vector3d *> m_normal;
	Material *m_mtl;
};

class Model : public Shape
{
public:
	Model(const char *filename, MatLib *matlib);
	Model(FILE *fp, MatLib *matlib);
	~Model();

	void load(const char *filename);

	void draw() const;
private:
	float my_hits(const Ray &ray,
	              vector3d &normal,
	              Material **material);
	
	MatLib *m_matlib;
	
	int m_v_count, m_f_count;
	int m_n_count, m_t_count;
	vector3d *m_vertex;
	vector3d *m_normal;
	Face *m_face;

	int m_cur_v, m_cur_f;
	int m_cur_n, m_cur_t;
	string m_cur_mtl;

	/* Functions for loading */
	void corrupt(const char *filename);
	int whitespace(FILE *in);
	int readc(FILE *in, const char *filename);
	int get_line(FILE *in);
	void get_vertex(FILE *in, const char *filename);
	void get_face(FILE *in, const char *filename);
	void get_normal(FILE *in, const char *filename);
	void get_texture(FILE *in, const char *filename);
	void get_material(FILE *in, const char *filename);
	void get_mtl(FILE *in, const char *filename);
};

#endif
