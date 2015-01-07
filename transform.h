#ifndef __TRANSFORM_H
#define __TRANSFORM_H

#include "vector3d.h"
#include "ray.h"

class Transform
{
public:
	Transform(Transform *rhs);
	virtual ~Transform();
	
	void draw() const;
	vector3d apply(const vector3d &v) const;
	vector3d apply_unit(const vector3d &v) const;
	vector3d invert(const vector3d &v) const;
	vector3d invert_unit(const vector3d &v) const;

	Ray apply(const Ray &ray) const;
	Ray invert(const Ray &ray) const;
protected:
	virtual void my_draw() const = 0;
	virtual vector3d my_apply(const vector3d &v) const = 0;
	virtual vector3d my_apply_unit(const vector3d &v) const = 0;
	virtual vector3d my_invert(const vector3d &v) const = 0;
	virtual vector3d my_invert_unit(const vector3d &v) const = 0;
private:
	Transform *m_rhs;
};

class IdentityTransform : public Transform
{
public:
    IdentityTransform() : Transform(NULL) {}
	~IdentityTransform() {}
private:
	void my_draw() const {}
	vector3d my_apply(const vector3d &v) const { return v; }
	vector3d my_apply_unit(const vector3d &v) const { return v; }
	vector3d my_invert(const vector3d &v) const { return v; }
	vector3d my_invert_unit(const vector3d &v) const { return v; }
};

class TranslateTransform : public Transform
{
public:
	TranslateTransform(Transform *rhs,
	                   const vector3d &delta);
	~TranslateTransform();
private:
	void my_draw() const;
	vector3d my_apply(const vector3d &v) const;
	vector3d my_apply_unit(const vector3d &v) const;
	vector3d my_invert(const vector3d &v) const;
	vector3d my_invert_unit(const vector3d &v) const;

	vector3d m_delta;
};

class ScaleTransform : public Transform
{
public:
	ScaleTransform(Transform *rhs,
	               const vector3d &scale);
	~ScaleTransform();
private:
	void my_draw() const;
	vector3d my_apply(const vector3d &v) const;
	vector3d my_apply_unit(const vector3d &v) const;
	vector3d my_invert(const vector3d &v) const;
	vector3d my_invert_unit(const vector3d &v) const;

	vector3d m_scale;
};

class RotateTransform : public Transform
{
public:
	RotateTransform(Transform *rhs,
	                float theta,
	                const vector3d &axis);
	~RotateTransform();
private:
	void my_draw() const;
	vector3d my_apply(const vector3d &v) const;
	vector3d my_apply_unit(const vector3d &v) const;
	vector3d my_invert(const vector3d &v) const;
	vector3d my_invert_unit(const vector3d &v) const;

	float m_theta;
	vector3d m_axis;
};

#endif
