#include "singletonmanager.h"

#include "datamanager.h"
#include "resourcemanager.h"

SingletonManager::SingletonManager()
{
	DataManager::get_instance();
	ResourceManager::get_instance();
}

SingletonManager::~SingletonManager()
{
	delete DataManager::get_instance();
	delete ResourceManager::get_instance();
}
