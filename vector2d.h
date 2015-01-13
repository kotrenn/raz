#ifndef __VECTOR2D_H
#define __VECTOR2D_H

#include <iostream>

#include "common.h"
#include "vector3d.h"

using namespace std;

class vector3d;

class vector2d
{
public:
	vector2d();
	vector2d(float x, float y);
	vector2d(const vector2d &other);
	vector2d(const vector3d &other);
	~vector2d();
    
	vector2d &operator=(const vector2d &rhs);
	const float &operator[](int i) const;
	float &operator[](int i);
	const vector2d operator+(const vector2d &rhs) const;
	const vector2d operator+=(const vector2d &rhs);
	const vector2d operator-(const vector2d &rhs) const;
	const vector2d operator-=(const vector2d &rhs);
	const vector2d operator*(float rhs) const;
	const vector2d operator*=(float rhs);
	const vector2d operator/(float rhs) const;
	const vector2d operator/=(float rhs);
	float operator*(const vector2d &rhs) const;
	float operator%(const vector2d &rhs) const;
	bool operator==(const vector2d &rhs) const;
	bool operator!=(const vector2d &rhs) const;

	float get_x() const { return m_x; }
	float get_y() const { return m_y; }
	void set_x(float x) { m_x = x; }
	void set_y(float y) { m_y = y; }
	float norm() const;
	void normalize();
private:
	float m_x, m_y;
};

std::ostream &operator<<(std::ostream &out, const vector2d &rhs);
const vector2d operator*(float lhs, const vector2d &rhs);

#endif
