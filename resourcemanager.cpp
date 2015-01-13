#include "resourcemanager.h"

ResourceManager *ResourceManager::m_instance = NULL;

ResourceManager *ResourceManager::get_instance()
{
	if (m_instance == NULL)
		m_instance = new ResourceManager();
	return m_instance;
}

ResourceManager::ResourceManager()
	:m_images(),
	 m_font(NULL)
{
}

ResourceManager::~ResourceManager()
{
	delete m_font;

	for (auto si : m_images)
		delete si.second;
}

Surface *ResourceManager::get_image(const char *filename)
{
	string file_str(filename);
	if (m_images.find(file_str) == m_images.end())
	{
		char fullname[80];
		snprintf(fullname, 80, "images/%s", filename);
		m_images[file_str] = new Surface(fullname);
	}
	return m_images[file_str];
}

Font *ResourceManager::get_font()
{
	if (m_font == NULL)
		m_font = new Font("font.png");
	return m_font;
}
