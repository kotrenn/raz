#include "ray.h"

Ray::Ray()
	:m_loc(),
	 m_dir()
{
}

Ray::Ray(const vector3d &loc, const vector3d &dir)
	:m_loc(loc),
	 m_dir(dir)
{
}

Ray::~Ray()
{
}

vector3d Ray::compute(float t) const
{
	return m_loc + t * m_dir;
}
