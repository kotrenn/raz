#ifndef __SHAPE_2D_H
#define __SHAPE_2D_H

#include "vector2d.h"

class vector2d;

class Circle;
class Line;
class Rect;

class Shape2d
{
public:
	virtual ~Shape2d();
	virtual bool collides(const Shape2d *shape2d) const = 0;
	virtual bool visit_collides(const Circle *circle) const = 0;
	virtual bool visit_collides(const Line *line) const = 0;
	virtual bool visit_collides(const Rect *rect) const = 0;
	virtual vector2d get_center() const = 0;
	virtual const Shape2d &operator+=(const vector2d &rhs) = 0;
	void fix_rect(const Rect &rect, vector2d *v = NULL);
	virtual void fix_rect_x(const Rect &rect, vector2d *v) = 0;
	virtual void fix_rect_y(const Rect &rect, vector2d *v) = 0;
	virtual void print() const = 0;
};

class Circle : public Shape2d
{
public:
	Circle();
	Circle(float x, float y, float r);
	Circle(const vector2d &c, float r);

	const float &operator[](int i) const;
	float &operator[](int i);

	bool collides(const Shape2d *shape2d) const;
	bool visit_collides(const Circle *circle) const;
	bool visit_collides(const Line *line) const;
	bool visit_collides(const Rect *rect) const;
	vector2d get_center() const;
	const Shape2d &operator+=(const vector2d &rhs);
	void fix_rect_x(const Rect &rect, vector2d *v = NULL);
	void fix_rect_y(const Rect &rect, vector2d *v = NULL);
	void print() const;

	float get_x() const { return m_x; }
	float get_y() const { return m_y; }
	float get_r() const { return m_r; }
	void set_x(float x) { m_x = x; }
	void set_y(float y) { m_y = y; }
	void set_r(float r) { m_r = r; }
private:
	float m_x, m_y;
	float m_r;
};

class Line : public Shape2d
{
public:
	Line();
	Line(float x1, float y1, float x2, float y2);
	Line(const vector2d &p1, const vector2d &p2);

	const float &operator[](int i) const;
	float &operator[](int i);

	bool collides(const Shape2d *shape2d) const;
	bool visit_collides(const Circle *circle) const;
	bool visit_collides(const Line *line) const;
	bool visit_collides(const Rect *rect) const;
	vector2d get_center() const;
	const Shape2d &operator+=(const vector2d &rhs);
	void fix_rect_x(const Rect &rect, vector2d *v = NULL);
	void fix_rect_y(const Rect &rect, vector2d *v = NULL);
	void print() const;

	Rect get_bounds() const;
	float len() const;

	float get_x1() const { return m_x1; }
	float get_y1() const { return m_y1; }
	float get_x2() const { return m_x2; }
	float get_y2() const { return m_y2; }
	float get_dx() const { return m_x2 - m_x1; }
	float get_dy() const { return m_y2 - m_y1; }
	void set_x1(float x1) { m_x1 = x1; }
	void set_y1(float y1) { m_y1 = y1; }
	void set_x2(float x2) { m_x2 = x2; }
	void set_y2(float y2) { m_y2 = y2; }
private:
	float m_x1, m_y1;
	float m_x2, m_y2;
};

class Rect : public Shape2d
{
public:
	Rect();
	Rect(float x, float y, float w, float h);

	const float &operator[](int i) const;
	float &operator[](int i);

	bool collides(const Shape2d *shape2d) const;
	bool visit_collides(const Circle *circle) const;
	bool visit_collides(const Line *line) const;
	bool visit_collides(const Rect *rect) const;
	vector2d get_center() const;
	const Shape2d &operator+=(const vector2d &rhs);
	void fix_rect_x(const Rect &rect, vector2d *v = NULL);
	void fix_rect_y(const Rect &rect, vector2d *v = NULL);
	void print() const;

	bool contains(float x, float y) const;

	float get_x() const { return m_x; }
	float get_y() const { return m_y; }
	float get_w() const { return m_w; }
	float get_h() const { return m_h; }
	void set_x(float x) { m_x = x; }
	void set_y(float y) { m_y = y; }
	void set_w(float w) { m_w = w; }
	void set_h(float h) { m_h = h; }
private:
	float m_x, m_y;
	float m_w, m_h;
};

#endif
