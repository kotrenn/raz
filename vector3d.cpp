#include "vector3d.h"

vector3d::vector3d()
	: m_x(0), m_y(0), m_z(0)
{
}

vector3d::vector3d(float x, float y, float z)
	: m_x(x), m_y(y), m_z(z)
{
}

vector3d::vector3d(const vector3d &other)
	: m_x(other.m_x), m_y(other.m_y), m_z(other.m_z)
{
}

vector3d::~vector3d()
{
}

const vector3d &vector3d::operator=(const vector3d &rhs)
{
	m_x = rhs.m_x;
	m_y = rhs.m_y;
	m_z = rhs.m_z;
	return *this;
}

const float &vector3d::operator[](int i) const
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else
		return m_z;
}

float &vector3d::operator[](int i)
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else
		return m_z;
}

const vector3d vector3d::operator+(const vector3d &rhs) const
{
	vector3d ret(m_x, m_y, m_z);
	ret.m_x += rhs.m_x;
	ret.m_y += rhs.m_y;
	ret.m_z += rhs.m_z;
	return ret;
}

const vector3d vector3d::operator+=(const vector3d &rhs)
{
	m_x += rhs.m_x;
	m_y += rhs.m_y;
	m_z += rhs.m_z;
	return *this;
}

const vector3d vector3d::operator-(const vector3d &rhs) const
{
	vector3d ret(m_x, m_y, m_z);
	ret.m_x -= rhs.m_x;
	ret.m_y -= rhs.m_y;
	ret.m_z -= rhs.m_z;
	return ret;
}

const vector3d vector3d::operator-=(const vector3d &rhs)
{
	m_x -= rhs.m_x;
	m_y -= rhs.m_y;
	m_z -= rhs.m_z;
	return *this;
}

const vector3d vector3d::operator*(float rhs) const
{
	vector3d ret(m_x, m_y, m_z);
	ret.m_x *= rhs;
	ret.m_y *= rhs;
	ret.m_z *= rhs;
	return ret;
}

const vector3d operator*(float lhs, const vector3d &rhs)
{
	vector3d ret(lhs * rhs[0],
	             lhs * rhs[1],
	             lhs * rhs[2]);
	return ret;
}

const vector3d vector3d::operator*=(float rhs)
{
	m_x *= rhs;
	m_y *= rhs;
	m_z *= rhs;
	return *this;
}

const vector3d vector3d::operator/(float rhs) const
{
	vector3d ret(m_x, m_y, m_z);
	ret.m_x /= rhs;
	ret.m_y /= rhs;
	ret.m_z /= rhs;
	return ret;
}

const vector3d vector3d::operator/=(float rhs)
{
	m_x /= rhs;
	m_y /= rhs;
	m_z /= rhs;
	return *this;
}

float vector3d::operator*(const vector3d &rhs) const
{
	float dot =
		m_x * rhs.m_x +
		m_y * rhs.m_y +
		m_z * rhs.m_z;
	return dot;
}

const vector3d vector3d::operator%(const vector3d &rhs) const
{
	vector3d ret;
	ret.m_x = m_y * rhs.m_z - m_z * rhs.m_y;
	ret.m_y = m_z * rhs.m_x - m_x * rhs.m_z;
	ret.m_z = m_x * rhs.m_y - m_y * rhs.m_x;
	return ret;
}

const vector3d vector3d::operator^(const vector3d &rhs) const
{
	vector3d ret;
	ret.m_x = m_x * rhs.m_x;
	ret.m_y = m_y * rhs.m_y;
	ret.m_z = m_z * rhs.m_z;
	return ret;
}

bool vector3d::operator==(const vector3d &rhs) const
{
	bool ret =
		fabs(m_x - rhs.m_x) < EPSILON &&
		fabs(m_y - rhs.m_y) < EPSILON &&
		fabs(m_z - rhs.m_z) < EPSILON;
	return ret;
}

bool vector3d::operator!=(const vector3d &rhs) const
{
	bool ret =
		fabs(m_x - rhs.m_x) < EPSILON &&
		fabs(m_y - rhs.m_y) < EPSILON &&
		fabs(m_z - rhs.m_z) < EPSILON;
	return !ret;
}

float vector3d::norm() const
{
	return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

void vector3d::normalize()
{
	float mag = norm();
	if (mag > EPSILON)
	{
		m_x /= mag;
		m_y /= mag;
		m_z /= mag;
	}
}

const vector3d vector3d::rotate(float theta, const vector3d &v) const
{
	float t = theta * M_PI / 180.0;
	float c = cos(t);
	float s = sin(t);
	vector3d row0(c + m_x * m_x * (1 - c),
	              m_x * m_y * (1 - c) - m_z * s,
	              m_x * m_z * (1 - c) + m_y * s);
	vector3d row1(m_y * m_x * (1 - c) + m_z * s,
	              c + m_y * m_y * (1 - c),
	              m_y * m_z * (1 - c) - m_x * s);
	vector3d row2(m_z * m_x * (1 - c) - m_y * s,
	              m_z * m_y * (1 - c) + m_x * s,
	              c + m_z * m_z * (1 - c));
	vector3d ret(row0 * v, row1 * v, row2 * v);
	return ret;
}
