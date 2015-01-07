#ifndef __CAMERA_H
#define __CAMERA_H

#include "scene.h"

class Camera
{
public:
	Camera(int rows, int cols, Scene *scene);
	~Camera();

	void setup();
	void draw() const;
	
	void render(bool less = false);
	void draw_image();

	void key_func(unsigned char key, bool rendering);
	void motion(int dx, int dy, bool rendering);

	bool is_fuzzy() const { return m_fuzzy; }
	void set_fuzzy(bool fuzzy) { m_fuzzy = fuzzy; }
private:
	vector3d *get_pixel(int row, int col);

	Scene *m_scene;
	vector3d *m_picture;
	int m_rows;
	int m_cols;
	float m_left;
	float m_right;
	float m_bottom;
	float m_top;

	vector3d m_loc;
	vector3d m_up;
	vector3d m_at;

	int m_reflect;

	float m_theta_x;
	float m_theta_y;

	vector3d get_loc() const;
	vector3d get_up() const;
	vector3d get_at() const;

	bool m_fuzzy;
};

#endif
