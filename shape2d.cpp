#include "shape2d.h"

Shape2d::~Shape2d()
{
}

void Shape2d::fix_rect(const Rect &rect, vector2d *v)
{
	fix_rect_x(rect, v);
	fix_rect_y(rect, v);
}



Circle::Circle()
	:m_x(0), m_y(0),
	 m_r(0)
{
}

Circle::Circle(float x, float y, float r)
	:m_x(x), m_y(y),
	 m_r(r)
{
}

Circle::Circle(const vector2d &c, float r)
	:m_x(c[0]), m_y(c[1]),
	 m_r(r)
{
}

const float &Circle::operator[](int i) const
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else
		return m_r;
}

float &Circle::operator[](int i)
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else
		return m_r;
}

bool Circle::collides(const Shape2d *shape2d) const
{
	return shape2d->visit_collides(this);
}

bool Circle::visit_collides(const Circle *circle) const
{
	float dx = m_x - circle->m_x;
	float dy = m_y - circle->m_y;
	float r = m_r + circle->m_r;
	return dx * dx + dy * dy <= r * r;
}

bool Circle::visit_collides(const Line *line) const
{
	vector2d u(line->get_dx(), line->get_dy());
	vector2d R(m_x - line->get_x1(), m_y - line->get_y1());
	vector2d R2(m_x - line->get_x2(), m_y - line->get_y2());

	if (R * R < m_r * m_r)
		return true;
	if (R2 * R2 < m_r * m_r)
		return true;

	float len2 = u * u;
	u.normalize();

	float Rdotu = R * u;
	if (Rdotu < 0)
		return false;

	vector2d proj = Rdotu * u;
	if (proj * proj > len2)
		return false;

	vector2d d = R - proj;
	if (d * d > m_r * m_r)
		return false;
	
	return true;
}

bool Circle::visit_collides(const Rect *rect) const
{
	float cx, cy;
	if (m_x < rect->get_x())
		cx = rect->get_x();
	else if (m_x > rect->get_x() + rect->get_w())
		cx = rect->get_x() + rect->get_w();
	else
		cx = m_x;
	if (m_y < rect->get_y())
		cy = rect->get_y();
	else if (m_y > rect->get_y() + rect->get_h())
		cy = rect->get_y() + rect->get_h();
	else
		cy = m_y;
	float dx = m_x - cx;
	float dy = m_y - cy;
	return dx * dx + dy * dy <= m_r * m_r;
}

vector2d Circle::get_center() const
{
	return vector2d(m_x, m_y);
}

const Shape2d &Circle::operator+=(const vector2d &rhs)
{
	m_x += rhs[0];
	m_y += rhs[1];
	return *this;
}

void Circle::fix_rect_x(const Rect &rect, vector2d *v)
{
	float vx = 0;
	if (v != NULL)
		vx = v->get_x();
	
	if (m_x - m_r < rect[0] && vx <= 0)
	{
		if (v != NULL)
			v->set_x(-1 * v->get_x());
		m_x = rect[0] + m_r;
	}
	else if (m_x + m_r > rect[0] + rect[2] && vx >= 0)
	{
		if (v != NULL)
			v->set_x(-1 * v->get_x());
		m_x = rect[0] + rect[2] - m_r;
	}
}
	
void Circle::fix_rect_y(const Rect &rect, vector2d *v)
{
	float vy = 0;
	if (v != NULL)
		vy = v->get_y();
	
	if (m_y - m_r < rect[1] && vy <= 0)
	{
		if (v != NULL)
			v->set_y(-1 * v->get_y());
		m_y = rect[1] + m_r;
	}
	else if (m_y + m_r > rect[1] + rect[3] && vy >= 0)
	{
		if (v != NULL)
			v->set_y(-1 * v->get_y());
		m_y = rect[1] + rect[3] - m_r;
	}
}

void Circle::print() const
{
	printf("circle: [%f %f] %f\n", m_x, m_y, m_r);
}



Line::Line()
	:m_x1(0), m_y1(0),
	 m_x2(0), m_y2(0)
{
}

Line::Line(float x1, float y1, float x2, float y2)
	:m_x1(x1), m_y1(y1),
	 m_x2(x2), m_y2(y2)
{
}

Line::Line(const vector2d &p1, const vector2d &p2)
	:m_x1(p1[0]), m_y1(p1[1]),
	 m_x2(p2[0]), m_y2(p2[1])
{
}

const float &Line::operator[](int i) const
{
	if (i == 0)
		return m_x1;
	else if (i == 1)
		return m_y1;
	else if (i == 2)
		return m_x2;
	else
		return m_y2;
}

float &Line::operator[](int i)
{
	if (i == 0)
		return m_x1;
	else if (i == 1)
		return m_y1;
	else if (i == 2)
		return m_x2;
	else
		return m_y2;
}

bool Line::collides(const Shape2d *shape2d) const
{
	return shape2d->visit_collides(this);
}

bool Line::visit_collides(const Circle *circle) const
{
	return circle->visit_collides(this);
}

bool Line::visit_collides(const Line *line) const
{
	float x3 = line->m_x1, y3 = line->m_x2;
	float x4 = line->m_x2, y4 = line->m_y2;
	/*
	float denom  = (y4 - y3) * (m_x2 - m_x1);
	float numera = (x4 - x3) *	(m_y1 - y3);
	float numerb = (m_x2 - m_x1) * (m_y2 - y3);
	denom  -= (line->m_x2 - line->m_x1) * (m_y2 - m_y1);
	numera -= (line->m_y2 - line->m_y1) * (m_x1 - line->m_x1);
	numerb -= (m_y2 - m_y1) * (m_x1 - line->m_x1);
	*/
	float x1 = m_x1, y1 = m_y1;
	float x2 = m_x2, y2 = m_y2;
	float denom  = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
	float numera = (x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3);
	float numerb = (x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3);
	if (fabs(denom) < EPSILON)
	{
		if (fabs(numera) < EPSILON && fabs(numerb) < EPSILON)
		{
			Rect b1 = get_bounds();
			Rect b2 = line->get_bounds();
			if (b1.collides(&b2))
				return true;
			/*
			return bounds.contains(line->m_x1, line->m_y1) ||
				bounds.contains(line->m_x2, line->m_y2);
			*/
		}
		return false;
	}
	float ua = numera / denom;
	float ub = numerb / denom;
	return 0 < ua && ua < 1 && 0 < ub && ub < 1;
}

bool Line::visit_collides(const Rect *rect) const
{
	Line line1(rect->get_x(), rect->get_y(),
	           rect->get_x() + rect->get_w() - 1,
	           rect->get_y());
	Line line2(rect->get_x(), rect->get_y(),
	           rect->get_x(),
	           rect->get_y() + rect->get_h() - 1);
	Line line3(rect->get_x() + rect->get_w() - 1,
	           rect->get_y(),
	           rect->get_x() + rect->get_w() - 1,
	           rect->get_y() + rect->get_h() - 1);
	Line line4(rect->get_x(),
	           rect->get_y() + rect->get_h() - 1,
	           rect->get_x() + rect->get_w() - 1,
	           rect->get_y() + rect->get_h() - 1);
	return rect->contains(m_x1, m_y1) ||
		rect->contains(m_x2, m_y2) ||
		visit_collides(&line1) || visit_collides(&line2) ||
		visit_collides(&line3) || visit_collides(&line4);
}

vector2d Line::get_center() const
{
	return vector2d(0.5 * (m_x1 + m_x2),
	                0.5 * (m_y1 + m_y2));
}

const Shape2d &Line::operator+=(const vector2d &rhs)
{
	m_x1 += rhs[0];
	m_y1 += rhs[1];
	m_x2 += rhs[0];
	m_y2 += rhs[1];
	return *this;
}

void Line::fix_rect_x(const Rect &rect, vector2d *v)
{
	float vx = 0;
	if (v != NULL)
		vx = v->get_x();
	
	if (m_x1 < rect[0] || m_x2 < rect[0])
	{
		if (vx <= 0)
		{
			if (v != NULL)
				v->set_x(-1 * v->get_x());
			float min_x = min(m_x1, m_x2);
			m_x1 += rect[0] - min_x;
			m_x2 += rect[0] - min_x;
		}
	}
	else if (m_x1 > rect[0] + rect[2] || m_x2 > rect[0] + rect[2])
	{
		if (vx >= 0)
		{
			if (v != NULL)
				v->set_x(-1 * v->get_x());
			float max_x = max(m_x1, m_x2);
			m_x1 -= max_x - rect[0] - rect[2];
			m_x2 -= max_x - rect[0] - rect[2];
		}
	}
}
	
void Line::fix_rect_y(const Rect &rect, vector2d *v)
{
	float vy = 0;
	if (v != NULL)
		vy = v->get_y();
	
	if (m_y1 < rect[1] || m_y2 < rect[1])
	{
		if (vy <= 0)
		{
			if (v != NULL)
				v->set_y(-1 * v->get_y());
			float min_y = min(m_y1, m_y2);
			m_y1 += rect[1] - min_y;
			m_y2 += rect[1] - min_y;
		}
	}
	else if (m_y1 > rect[1] + rect[3] || m_y2 > rect[1] + rect[3])
	{
		if (vy >= 0)
		{
			if (v != NULL)
				v->set_y(-1 * v->get_y());
			float max_y = max(m_y1, m_y2);
			m_y1 -= max_y - rect[1] - rect[3];
			m_y2 -= max_y - rect[1] - rect[3];
		}
	}
}

void Line::print() const
{
	printf("line: [%f %f] [%f %f]\n",
	       m_x1, m_y1, m_x2, m_y2);
}

Rect Line::get_bounds() const
{
	float min_x = min(m_x1, m_x2);
	float min_y = min(m_y1, m_y2);
	float max_x = max(m_x1, m_x2);
	float max_y = max(m_y1, m_y2);
	Rect bounds(min_x, min_y,
	            max_x - min_x + 1,
	            max_y - min_y + 1);
	return bounds;
}

float Line::len() const
{
	vector2d v(m_x2 - m_x1, m_y2 - m_y1);
	return v.norm();
}



Rect::Rect()
	:m_x(0), m_y(0),
	 m_w(0), m_h(0)
{
}

Rect::Rect(float x, float y, float w, float h)
	:m_x(x), m_y(y),
	 m_w(w), m_h(h)
{
}

const float &Rect::operator[](int i) const
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else if (i == 2)
		return m_w;
	else
		return m_h;
}

float &Rect::operator[](int i)
{
	if (i == 0)
		return m_x;
	else if (i == 1)
		return m_y;
	else if (i == 2)
		return m_w;
	else
		return m_h;
}

bool Rect::collides(const Shape2d *shape2d) const
{
	return shape2d->visit_collides(this);
}

bool Rect::visit_collides(const Circle *circle) const
{
	return circle->visit_collides(this);
}

bool Rect::visit_collides(const Line *line) const
{
	return line->visit_collides(this);
}

bool Rect::visit_collides(const Rect *rect) const
{
	return !(
		m_x >= rect->m_x + rect->m_w ||
		m_y >= rect->m_y + rect->m_h ||
		rect->m_x >= m_x + m_w ||
		rect->m_y >= m_y + m_h);
}

vector2d Rect::get_center() const
{
	return vector2d(m_x + 0.5 * m_w,
	                m_y + 0.5 * m_h);
}

const Shape2d &Rect::operator+=(const vector2d &rhs)
{
	m_x += rhs[0];
	m_y += rhs[1];
	return *this;
}

void Rect::fix_rect_x(const Rect &rect, vector2d *v)
{
	float vx = 0;
	if (v != NULL)
		vx = v->get_x();
	
	if (m_x < rect[0] && vx <= 0)
	{
		if (v != NULL)
			v->set_x(-1 * v->get_x());
		m_x = rect[0];
	}
	else if (m_x + m_w > rect[0] + rect[2] && vx >= 0)
	{
		if (v != NULL)
			v->set_x(-1 * v->get_x());
		m_x -= m_x + m_w - rect[0] - rect[2];
	}
}
	
void Rect::fix_rect_y(const Rect &rect, vector2d *v)
{
	float vy = 0;
	if (v != NULL)
		vy = v->get_y();
	
	if (m_y < rect[1] && vy <= 0)
	{
		if (v != NULL)
			v->set_y(-1 * v->get_y());
		m_y = rect[1];
	}
	else if (m_y + m_h > rect[1] + rect[3] && vy >= 0)
	{
		if (v != NULL)
			v->set_y(-1 * v->get_y());
		m_y -= m_y + m_h - rect[1] - rect[3];
	}
}

void Rect::print() const
{
	printf("rect: [%f %f] [%f %f]\n",
	       m_x, m_y, m_w, m_h);
}

bool Rect::contains(float x, float y) const
{
	float min_x = m_x;
	float min_y = m_y;
	float max_x = m_x + m_w - 1;
	float max_y = m_y + m_h - 1;
	return min_x <= x && x <= max_x && min_y <= y && y <= max_y;
}
