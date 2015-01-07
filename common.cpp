#include "common.h"

float Math::cube_root(float x)
{
	if (x < 0)
		return -pow(-x, 1.0 / 3.0);
	else
		return pow(x, 1.0 / 3.0);
}

set<float> Math::quadratic(float a, float b, float c)
{
	set<float> ret;
	float D = b * b - 4 * a * c;
	if (D < 0)
		return ret;
	if (D == 0)
	{
		ret.insert(-b / (2 * a));
		return ret;
	}
	D = sqrt(D);
	ret.insert((-b + D) / (2 * a));
	ret.insert((-b - D) / (2 * a));
	return ret;
}

set<float> Math::cubic(float a, float b, float c)
{
	set<float> ret;
	float p = (-1.0 / 9.0) * a * a + (1.0 / 3.0) * b;
	float q = (1.0 / 27.0) * a * a * a - (1.0 / 6.0) * a * b + 0.5 * c;
	float D = -(p * p * p + q * q);
	if (D <= 0)
	{
		float r = cube_root(-q + sqrt(-D));
		float s = cube_root(-q - sqrt(-D));
		ret.insert(r + s);
		if (D == 0)
			ret.insert(-r);
	}
	else
	{
		float ratio = (-q) / (sqrt(-(p * p * p)));
		float theta = (1.0 / 3.0) * acos(ratio);
		float sp = sqrt(-p);
		ret.insert(2 * sp * cos(theta));
		ret.insert(2 * sp * cos(theta + 2 * M_PI / 3.0));
		ret.insert(2 * sp * cos(theta - 2 * M_PI / 3.0));
	}
	set<float> ret2;
	set<float>::iterator it;
	for (it = ret.begin(); it != ret.end(); it++)
		ret2.insert(*it - a / 3);
	return ret2;
}

set<float> Math::quartic(float a, float b, float c, float d)
{
	set<float> ret;
	float a2 = a * a;
	float p = (-3 / 8.0) * a2 + b;
	float q = (1 / 8.0) * a2 * a - 0.5 * a * b + c;
	float r = (-3 / 256.0) * a2 * a2 + (1 / 16.0) * a2 * b;
	r += -0.25 * a * c + d;
	set<float> roots = cubic(-p / 2, -r, (4 * r * p - q * q) / 8);
	float y = *(roots.begin());
	float r1 = 2 * y - p;
	float r2 = y * y - r;
	if (r1 >= 0 && r2 >= 0)
	{
		if (q >= 0)
		{
			set<float> q1 = quadratic(1,  sqrt(r1), y - sqrt(r2));
			set<float> q2 = quadratic(1, -sqrt(r1), y + sqrt(r2));
			set<float>::iterator it;
			for (it = q1.begin(); it != q1.end(); it++)
				ret.insert(*it);
			for (it = q2.begin(); it != q2.end(); it++)
				ret.insert(*it);
		}
		else
		{
			set<float> q1 = quadratic(1,  sqrt(r1), y + sqrt(r2));
			set<float> q2 = quadratic(1, -sqrt(r1), y - sqrt(r2));
			set<float>::iterator it;
			for (it = q1.begin(); it != q1.end(); it++)
				ret.insert(*it);
			for (it = q2.begin(); it != q2.end(); it++)
				ret.insert(*it);
		}
	}
	set<float> ret2;
	set<float>::iterator it;
	for (it = ret.begin(); it != ret.end(); it++)
		ret2.insert(*it - a / 4);
	return ret2;
}
