#include "camera.h"

Camera::Camera(int rows, int cols, Scene *scene)
	:m_scene(scene),
	 m_picture(NULL),
	 m_rows(rows),
	 m_cols(cols),
	 m_left(0),
	 m_right(0),
	 m_bottom(0),
	 m_top(0),
	 m_loc(),
	 m_up(),
	 m_at(),
	 m_reflect(0),
	 m_theta_x(0),
	 m_theta_y(0),
	 m_fuzzy(false)
{
	FILE *project_fp;
	if ((project_fp = fopen("project.txt", "r")) == NULL)
	{
		m_left = -1;
		m_right = 1;
		m_bottom = -1;
		m_top = 1;
	}
	else
	{
		char buf[80];
		float val;
		while (fscanf(project_fp, "%s %f", buf, &val) == 2)
		{
			if (strcmp(buf, "left") == 0)
				m_left = val;
			else if (strcmp(buf, "right") == 0)
				m_right = val;
			else if (strcmp(buf, "bottom") == 0)
				m_bottom = val;
			else if (strcmp(buf, "top") == 0)
				m_top = val;
		}
		fclose(project_fp);
	}
	
	FILE *view_fp;
	if ((view_fp = fopen("view.txt", "r")) == NULL)
	{
		m_loc = vector3d(0, 0, -2);
		m_up = vector3d(0, 1, 0);
		m_at = vector3d(0, 0, 1);
	}
	else
	{
		char buf[80];
		float val1, val2, val3;
		while (fscanf(project_fp,
		              "%s %f %f %f",
		              buf, &val1, &val2, &val3) == 4)
		{
			if (strcmp(buf, "loc") == 0)
				m_loc = vector3d(val1, val2, val3);
			else if (strcmp(buf, "up") == 0)
				m_up = vector3d(val1, val2, val3);
			else if (strcmp(buf, "at") == 0)
				m_at = vector3d(val1, val2, val3);
		}
		fclose(view_fp);
	}

	FILE *reflect_fp;
	if ((reflect_fp = fopen("reflect.txt", "r")) == NULL)
		m_reflect = 0;
	else
	{
		char buf[80];
		int val;
		while (fscanf(reflect_fp, "%s %d", buf, &val) == 2)
		{
			if (strcmp(buf, "depth") == 0)
				m_reflect = val;
		}
		fclose(reflect_fp);
	}

	m_up.normalize();

	m_picture = new vector3d[m_rows * m_cols];
}

Camera::~Camera()
{
	delete m_picture;
}

vector3d *Camera::get_pixel(int row, int col)
{
	return m_picture + row * m_cols + col;
}

void Camera::setup()
{
	m_scene->setup();
}

void Camera::key_func(unsigned char key, bool rendering)
{
	bool updated = false;

	float dist = 0.1;
	if (key == 'w' || key == 'W')
	{
		vector3d dir = m_at - m_loc;
		dir.normalize();
		m_loc += dist * dir;
		m_at += dist * dir;
		
		updated = true;
	}
	if (key == 's' || key == 'S')
	{
		vector3d dir = m_at - m_loc;
		dir.normalize();
		m_loc -= dist * dir;
		m_at -= dist * dir;
		
		updated = true;
	}
	if (key == 'a' || key == 'A')
	{
		vector3d dir = m_at - m_loc;
		dir.normalize();
		dir = dir % m_up;
		m_loc -= dist * dir;
		m_at -= dist * dir;
		
		updated = true;
	}
	if (key == 'd' || key == 'D')
	{
		vector3d dir = m_at - m_loc;
		dir.normalize();
		dir = dir % m_up;
		m_loc += dist * dir;
		m_at += dist * dir;
		
		updated = true;
	}
	if (key == 'q' || key == 'Q')
	{
		m_loc -= dist * m_up;
		m_at -= dist * m_up;
	}
	if (key == 'e' || key == 'E')
	{
		m_loc += dist * m_up;
		m_at += dist * m_up;
	}
	if (key == 'r' || key == 'R')
		updated = true;

	if (updated && rendering)
	{
		m_fuzzy = true;
		render(true);
	}
}

void Camera::motion(int dx, int dy, bool rendering)
{
	if (abs(dx) + abs(dy) == 0)
		return;
	
	// 200 pixels = 1 quarter turn
	float ratio = 90.0 / 200.0;
	float theta_x = -dx * ratio;
	float theta_y =  dy * ratio;

	float limit = 90.0 - EPSILON;
	if (m_theta_y + theta_y < -limit)
		theta_y = -limit - m_theta_y;
	if (m_theta_y + theta_y > limit)
		theta_y = limit - m_theta_y;
	
	m_theta_x += theta_x;
	m_theta_y += theta_y;

	vector3d r;
	vector3d dir = m_at - m_loc;

	r = m_up;
	dir = r.rotate(theta_x, dir);
	m_at = m_loc + dir;
	
	r = dir % m_up;
	r.normalize();
	dir = r.rotate(theta_y, dir);
	m_at = m_loc + dir;

	if (rendering)
	{
		m_fuzzy = true;
		render(true);
	}
}

vector3d Camera::get_loc() const
{
	return m_loc;
}

vector3d Camera::get_up() const
{
	vector3d dir = m_at - m_loc;
	vector3d r = dir % m_up;
	vector3d up = r.rotate(m_theta_y, m_up);
	up.normalize();
	return up;
}

vector3d Camera::get_at() const
{
	return m_at;
}

void Camera::draw() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(m_left, m_right,
	          m_bottom, m_top,
	          1.0, 30.0);

	vector3d loc = get_loc();
	vector3d up = get_up();
	vector3d at = get_at();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	/*
	gluLookAt(m_loc[0], m_loc[1], m_loc[2],
	          m_at[0], m_at[1], m_at[2],
	          m_up[0], m_up[1], m_up[2]);
	*/
	gluLookAt(loc[0], loc[1], loc[2],
	          at[0], at[1], at[2],
	          up[0], up[1], up[2]);

	m_scene->draw();
}

void Camera::render(bool less)
{
	vector3d loc = get_loc();
	vector3d up = get_up();
	vector3d at = get_at();

	float view_dist = 1.0;
	vector3d v = at - loc;
	v.normalize();
	v *= view_dist;
	vector3d center = loc + v;
	vector3d bx = (v % up);
	bx.normalize();
	vector3d by = (bx % v);
	by.normalize();
	vector3d b = center;
	b += m_left * bx;
	b += m_bottom * by;

	int quality = 8;

	float width = m_right - m_left;
	float height = m_top - m_bottom;
	float dx = width  / m_cols;
	float dy = height / m_rows;
	for (int cur_x = 0; cur_x < m_cols; ++cur_x)
	{
		for (int cur_y = 0; cur_y < m_rows; ++cur_y)
		{
			bool compute = false;
			if (!less)
				compute = true;
			else if (cur_x % quality == 0 && cur_y % quality == 0)
				compute = true;
			if (compute)
			{
				vector3d p = b;
				p += cur_x * dx * bx;
				p += cur_y * dy * by;
				vector3d dir = (p - loc);
				dir.normalize();
				Ray ray(p, dir);
				vector3d color = m_scene->shoot(ray, NULL, m_reflect);
				*(get_pixel(cur_y, cur_x)) = color;
			}
			else
			{
				int tmp_x = quality * (cur_x / quality);
				int tmp_y = quality * (cur_y / quality);
				*(get_pixel(cur_y, cur_x)) = *(get_pixel(tmp_y, tmp_x));
			}
		}
	}
}

void Camera::draw_image()
{
	glBegin(GL_POINTS);
	
	for (int cur_x = 0; cur_x < m_cols; ++cur_x)
	{
		for (int cur_y = 0; cur_y < m_rows; ++cur_y)
		{
			vector3d color = *get_pixel(cur_y, cur_x);
			//color = vector3d((1.0 * cur_x) / m_cols, 0, (1.0 * cur_y) / m_rows);
			glColor3f(color[0], color[1], color[2]);
			glVertex2i(cur_x, cur_y);
		}
	}

	glEnd();
}
