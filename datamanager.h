#ifndef __DATA_MANAGER_H
#define __DATA_MANAGER_H

#include "common.h"
#include "vector2d.h"

class DataManager
{
public:
	static DataManager *get_instance();
	~DataManager();

	int get_int(const char *var);
	void get_str(const char *var, char *dst);
	vector2d get_vec(const char *var);
	float get_flt(const char *var);
private:
	DataManager();
	static DataManager *m_instance;

	static void error(const char *var);

	map<string, int> m_int;
	map<string, string> m_str;
	map<string, vector2d> m_vec;
	map<string, float> m_flt;
};

#endif
