#ifndef __RESOURCE_MANAGER_H
#define __RESOURCE_MANAGER_H

#include "surface.h"
#include "font.h"

class ResourceManager
{
public:
	static ResourceManager *get_instance();
	~ResourceManager();

	Surface *get_image(const char *filename);

	Font *get_font();
private:
	ResourceManager();
	static ResourceManager *m_instance;

	map<string, Surface *> m_images;
	Font *m_font;
};

#endif
