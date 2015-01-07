#ifndef __MAT_LIB_H
#define __MAT_LIB_H

#include "material.h"

class MatLib
{
public:
	MatLib();
	~MatLib();

	void open(const char *filename);
	Material *get(const string &name);
private:
	void corrupt(const char *filename);
	int whitespace(FILE *in);
	int readc(FILE *in, const char *filename);
	int get_line(FILE *in);
	void get_mtl(FILE *in, const char *filename);
	void get_att(FILE *in, const char *filename, const string &att);
	void get_shininess(FILE *in, const char *filename);
	
	map<string, Material *> m_materials;
	string m_cur_name;
	Material *m_cur;
	set<string> m_files;
};

#endif
