#include "vector2d.h"

vector2d::vector2d()
	:m_x(0), m_y(0)
{
}

vector2d::vector2d(float x, float y)
	:m_x(x), m_y(y)
{
}

vector2d::vector2d(const vector2d &other)
	:m_x(other.m_x), m_y(other.m_y)
{
}

vector2d::vector2d(const vector3d &other)
	:m_x(other[0]), m_y(other[1])
{
}

vector2d::~vector2d()
{
}

vector2d &vector2d::operator=(const vector2d &rhs)
{
	m_x = rhs.m_x;
	m_y = rhs.m_y;
	return *this;
}

std::ostream &operator<<(std::ostream &out, const vector2d &rhs)
{
	out.precision(3);
	return out << fixed << "[" << rhs[0] << ", " << rhs[1] << "]";
}

const float &vector2d::operator[](int i) const
{
	if (i == 0)
		return m_x;
	else
		return m_y;
}

float &vector2d::operator[](int i)
{
	if (i == 0)
		return m_x;
	else
		return m_y;
}

const vector2d vector2d::operator+(const vector2d &rhs) const
{
	vector2d ret(m_x, m_y);
	ret.m_x += rhs.m_x;
	ret.m_y += rhs.m_y;
	return ret;
}

const vector2d vector2d::operator+=(const vector2d &rhs)
{
	m_x += rhs.m_x;
	m_y += rhs.m_y;
	return *this;
}

const vector2d vector2d::operator-(const vector2d &rhs) const
{
	vector2d ret(m_x, m_y);
	ret.m_x -= rhs.m_x;
	ret.m_y -= rhs.m_y;
	return ret;
}

const vector2d vector2d::operator-=(const vector2d &rhs)
{
	m_x -= rhs.m_x;
	m_y -= rhs.m_y;
	return *this;
}

const vector2d vector2d::operator*(float rhs) const
{
	vector2d ret(m_x, m_y);
	ret.m_x *= rhs;
	ret.m_y *= rhs;
	return ret;
}

const vector2d operator*(float lhs, const vector2d &rhs)
{
	vector2d ret(lhs * rhs[0], lhs * rhs[1]);
	return ret;
}

const vector2d vector2d::operator*=(float rhs)
{
	m_x *= rhs;
	m_y *= rhs;
	return *this;
}

const vector2d vector2d::operator/(float rhs) const
{
	vector2d ret(m_x, m_y);
	ret.m_x /= rhs;
	ret.m_y /= rhs;
	return ret;
}

const vector2d vector2d::operator/=(float rhs)
{
	m_x /= rhs;
	m_y /= rhs;
	return *this;
}

float vector2d::operator*(const vector2d &rhs) const
{
	float dot = m_x * rhs.m_x + m_y * rhs.m_y;
	return dot;
}

float vector2d::operator%(const vector2d &rhs) const
{
	float ret = m_x * rhs.m_y - rhs.m_y * m_x;
	return ret;
}

bool vector2d::operator==(const vector2d &rhs) const
{
	bool ret =
		fabs(m_x - rhs.m_x) < EPSILON &&
		fabs(m_y - rhs.m_y) < EPSILON;
	return ret;
}

bool vector2d::operator!=(const vector2d &rhs) const
{
	bool ret =
		fabs(m_x - rhs.m_x) < EPSILON &&
		fabs(m_y - rhs.m_y) < EPSILON;
	return !ret;
}

float vector2d::norm() const
{
	return sqrt(m_x * m_x + m_y * m_y);
}

void vector2d::normalize()
{
	float mag = norm();
	if (mag > EPSILON)
	{
		m_x /= mag;
		m_y /= mag;
	}
}
