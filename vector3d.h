#ifndef __VECTOR3D_H
#define __VECTOR3D_H

#include "common.h"

class vector3d
{
public:
	vector3d();
	vector3d(float x, float y, float z);
	vector3d(const vector3d &other);
	~vector3d();
    
	const vector3d &operator=(const vector3d &rhs);
	const float &operator[](int i) const;
	float &operator[](int i);
	const vector3d operator+(const vector3d &rhs) const;
	const vector3d operator+=(const vector3d &rhs);
	const vector3d operator-(const vector3d &rhs) const;
	const vector3d operator-=(const vector3d &rhs);
	const vector3d operator*(float rhs) const;
	const vector3d operator*=(float rhs);
	const vector3d operator/(float rhs) const;
	const vector3d operator/=(float rhs);
	float operator*(const vector3d &rhs) const;
	const vector3d operator%(const vector3d &rhs) const;
	const vector3d operator^(const vector3d &rhs) const;
	bool operator==(const vector3d &rhs) const;
	bool operator!=(const vector3d &rhs) const;

	float get_x() const { return m_x; }
	float get_y() const { return m_y; }
	float get_z() const { return m_z; }
	void set_x(float x) { m_x = x; }
	void set_y(float y) { m_y = y; }
	void set_z(float z) { m_z = z; }
	float norm() const;
	void normalize();

	/* theta is in degrees */
	const vector3d rotate(float theta, const vector3d &v) const;
private:
	float m_x, m_y, m_z;
};

const vector3d operator*(float lhs, const vector3d &rhs);

#endif
