#ifndef __RAY_H
#define __RAY_H

#include "vector3d.h"

class Ray
{
public:
	Ray();
	Ray(const vector3d &pos, const vector3d &dir);
	~Ray();
	
	vector3d get_loc() const { return m_loc; }
	vector3d get_dir() const { return m_dir; }
	void set_loc(const vector3d &loc) { m_loc = loc; }
	void set_dir(const vector3d &dir) { m_dir = dir; }
	vector3d compute(float t) const;
private:
	vector3d m_loc;
	vector3d m_dir;
};

#endif
