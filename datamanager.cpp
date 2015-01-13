#include "datamanager.h"

DataManager *DataManager::m_instance = NULL;

DataManager *DataManager::get_instance()
{
	if (m_instance == NULL)
		m_instance = new DataManager();
	return m_instance;
}

DataManager::DataManager()
	:m_int(),
	 m_str(),
	 m_vec(),
	 m_flt()
{
	FILE *data;
	if ((data = fopen("data.txt", "r")) == NULL)
	{
		fprintf(stderr, "Could not open data.txt for reading\n");
		exit(1);
	}

	char type[81];
	char name[81];

	while (fscanf(data, "%80s %80s", type, name) > 0)
	{
		if (strcmp(type, "int") == 0)
		{
			int val;
			fscanf(data, "%d", &val);
			m_int[string(name)] = val;
		}
		else if (strcmp(type, "vec") == 0)
		{
			float x, y;
			fscanf(data, "%f %f", &x, &y);
			m_vec[string(name)] = vector2d(x, y);
		}
		else if (strcmp(type, "str") == 0)
		{
			char val[81];
			fscanf(data, "%80s", val);
			int i = 1;
			while (val[i])
			{
				val[i - 1] = val[i];
				++i;
			}
			val[i - 2] = '\0';
			m_str[string(name)] = string(val);
		}
		else if (strcmp(type, "flt") == 0)
		{
			float val;
			fscanf(data, "%f", &val);
			m_flt[string(name)] = val;
		}
	}

	fclose(data);

	fflush(stdout);
}

DataManager::~DataManager()
{
}

void DataManager::error(const char *var)
{
	fprintf(stderr, "Could not find value <%s> in data.txt\n", var);
	exit(1);
}

int DataManager::get_int(const char *var)
{
	string var_str(var);
	if (m_int.find(var_str) == m_int.end())
		error(var);
	return m_int[var_str];
}

void DataManager::get_str(const char *var, char *dst)
{
	string var_str(var);
	if (m_str.find(var_str) == m_str.end())
		error(var);
	strcpy(dst, m_str[var_str].c_str());
}

vector2d DataManager::get_vec(const char *var)
{
	string var_str(var);
	if (m_vec.find(var_str) == m_vec.end())
		error(var);
	return m_vec[var_str];
}

float DataManager::get_flt(const char *var)
{
	string var_str(var);
	if (m_flt.find(var_str) == m_flt.end())
		error(var);
	return m_flt[var_str];
}
