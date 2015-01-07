#include "transform.h"

Transform::Transform(Transform *rhs)
	:m_rhs(rhs)
{
}

Transform::~Transform()
{
	if (m_rhs != NULL)
		delete m_rhs;
}

void Transform::draw() const
{
	my_draw();
	if (m_rhs != NULL)
		m_rhs->draw();
}

vector3d Transform::apply(const vector3d &v) const
{
	vector3d ret = v;
	if (m_rhs != NULL)
		ret = m_rhs->apply(v);
	ret = my_apply(ret);
	return ret;
}

vector3d Transform::apply_unit(const vector3d &v) const
{
	vector3d ret = v;
	if (m_rhs != NULL)
		ret = m_rhs->apply_unit(v);
	ret = my_apply_unit(ret);
	return ret;
}

vector3d Transform::invert(const vector3d &v) const
{
	vector3d ret = v;
	ret = my_invert(ret);
	if (m_rhs != NULL)
		ret = m_rhs->invert(ret);
	return ret;
}

vector3d Transform::invert_unit(const vector3d &v) const
{
	vector3d ret = v;
	ret = my_invert_unit(ret);
	if (m_rhs != NULL)
		ret = m_rhs->invert_unit(ret);
	return ret;
}

Ray Transform::apply(const Ray &ray) const
{
	vector3d loc = apply(ray.get_loc());
	vector3d dir = apply_unit(ray.get_dir());
	return Ray(loc, dir);
}

Ray Transform::invert(const Ray &ray) const
{
	vector3d loc = invert(ray.get_loc());
	vector3d dir = invert_unit(ray.get_dir());
	return Ray(loc, dir);
}



TranslateTransform::TranslateTransform(Transform *rhs,
                                       const vector3d &delta)
	:Transform(rhs),
	 m_delta(delta)
{
}

TranslateTransform::~TranslateTransform()
{
}

void TranslateTransform::my_draw() const
{
	glTranslatef(m_delta[0], m_delta[1], m_delta[2]);
}

vector3d TranslateTransform::my_apply(const vector3d &v) const
{
	return v + m_delta;
}

vector3d TranslateTransform::my_apply_unit(const vector3d &v) const
{
	return v;
}

vector3d TranslateTransform::my_invert(const vector3d &v) const
{
	return v - m_delta;
}

vector3d TranslateTransform::my_invert_unit(const vector3d &v) const
{
	return v;
}



ScaleTransform::ScaleTransform(Transform *rhs,
                               const vector3d &scale)
	:Transform(rhs),
	 m_scale(scale)
{
}

ScaleTransform::~ScaleTransform()
{
}

void ScaleTransform::my_draw() const
{
	glScalef(m_scale[0], m_scale[1], m_scale[2]);
}

vector3d ScaleTransform::my_apply(const vector3d &v) const
{
	return vector3d(v[0] * m_scale[0],
	                v[1] * m_scale[1],
	                v[2] * m_scale[2]);
}

vector3d ScaleTransform::my_apply_unit(const vector3d &v) const
{
	return vector3d(v[0] / m_scale[0],
	                v[1] / m_scale[1],
	                v[2] / m_scale[2]);
}

vector3d ScaleTransform::my_invert(const vector3d &v) const
{
	return vector3d(v[0] / m_scale[0],
	                v[1] / m_scale[1],
	                v[2] / m_scale[2]);
}

vector3d ScaleTransform::my_invert_unit(const vector3d &v) const
{
	return vector3d(v[0] * m_scale[0],
	                v[1] * m_scale[1],
	                v[2] * m_scale[2]);
}



RotateTransform::RotateTransform(Transform *rhs,
                                 float theta,
                                 const vector3d &axis)
	:Transform(rhs),
	 m_theta(theta),
	 m_axis(axis)
{
	m_axis.normalize();
}

RotateTransform::~RotateTransform()
{
}

void RotateTransform::my_draw() const
{
	glRotatef(m_theta, m_axis[0], m_axis[1], m_axis[2]);
}

vector3d RotateTransform::my_apply(const vector3d &v) const
{
	return m_axis.rotate(m_theta, v);
}

vector3d RotateTransform::my_apply_unit(const vector3d &v) const
{
	return m_axis.rotate(m_theta, v);
}

vector3d RotateTransform::my_invert(const vector3d &v) const
{
	return m_axis.rotate(-m_theta, v);
}

vector3d RotateTransform::my_invert_unit(const vector3d &v) const
{
	return m_axis.rotate(-m_theta, v);
}
