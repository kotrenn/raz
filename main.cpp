/* Rob Seay
   rgseay@ncsu.edu
   CSC 562 - Assignment 2
*/

#include "camera.h"
#include "glslprogram.h"

#define STATE_RASTER 0
#define STATE_RAYCAST 1

int view_w;
int view_h;

int win_id;

int cur_state;

float theta_x;
float theta_y;
float d_theta;
float v_x;
float v_y;

int btn[3] = { 0 };
int mouse_x;
int mouse_y;

#define DELAY 6

int delay;

Scene *scene;
Camera *camera;

/* shader goes here */
GLSLProgram *simple;
float zoom;
float max_iters;
float radius;

void state_raster();
void state_raycast();

void init_glew();
void init_shader();

void init()
{
	theta_x = 0.0;
	theta_y = 0.0;
	d_theta = 2.0;
	v_x = 0.0;
	v_y = 0.0;

	delay = 0;

	scene = new Scene();
	camera = new Camera(view_h, view_w, scene);
	//camera->render();

	init_glew();
	init_shader();
	
	state_raster();
}

void init_glew()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "glewInitError\n");
		exit(1);
	}

	fprintf(stderr, "GLEW initialized OK\n");
	fprintf(stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void init_shader()
{
	simple = new GLSLProgram("mand.vert", "mand.frag");

	zoom = 2.0;
	max_iters = 20.0;
	radius = 0.0;

	simple->Use();
	simple->SetUniform("Zoom", zoom);
	simple->SetUniform("MaxIters", (int)max_iters);
	//simple->SetUniform("ConvergeColor", 0.5, 0.0, 0.5);
	simple->SetUniform("ConvergeColor", 0.0, 1.0, 0.5);
	simple->SetUniform("DivergeColor1", 1.0, 0.0, 0.0);
	simple->SetUniform("DivergeColor2", 1.0, 0.8, 0.0);
	simple->SetUniform("HitPos", 1.0, 0.0, 0.0);
	simple->SetUniform("Radius", radius);
}

void state_raster()
{
	cur_state = STATE_RASTER;

	camera->setup();
}

void state_raycast()
{
	cur_state = STATE_RAYCAST;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, view_w, 0, view_h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
}

void display_raster()
{
	zoom *= 0.995;
	max_iters += 0.05;
	radius += 0.03;
	if (radius > 4.0)
		radius = 0.0;
	simple->SetUniform("Zoom", zoom);
	simple->SetUniform("MaxIters", (int)max_iters);
	simple->SetUniform("Radius", (float)(2.0 + 2.0 * sin(radius * M_PI / 2.0)));
	camera->draw();
}

void display_raycast()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	camera->draw_image();
}

void display_func()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* enable shader */

	if (cur_state == STATE_RASTER)
		display_raster();
	else
		display_raycast();

	/* disable shader */

	glFlush();

	glutSwapBuffers();
}

void key_func(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27: //ESCAPE
		glutDestroyWindow(win_id);
		exit(0);
		break;
	case ' ':
		if (cur_state == STATE_RASTER)
			state_raycast();
		else
			state_raster();
	}

	bool rendering = cur_state == STATE_RAYCAST;
	camera->key_func(key, rendering);
	if (rendering)
		delay = DELAY;
}

void special_func(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		v_x = 0.0;
		v_y = -1.0;
		break;
	case GLUT_KEY_DOWN:
		v_x = 0.0;
		v_y = 1.0;
		break;
	case GLUT_KEY_LEFT:
		v_x = -1.0;
		v_y = 0.0;
		break;
	case GLUT_KEY_RIGHT:
		v_x = 1.0;
		v_y = 0.0;
		break;
	}
}

void mouse_func(int b, int s, int x, int y)
{
	if (s == GLUT_DOWN)
		btn[b] = 1;
	else
		btn[b] = 0;

	mouse_x = x;
	mouse_y = glutGet(GLUT_WINDOW_HEIGHT) - y;
}

void motion_func(int x, int y)
{
	if (!btn[0])
		return;

	bool rendering = cur_state == STATE_RAYCAST;

	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	camera->motion(x - mouse_x, y - mouse_y, rendering);

	mouse_x = x;
	mouse_y = y;

	if (rendering)
		delay = DELAY;

	glutPostRedisplay();
}

void idle_func()
{
	if (camera->is_fuzzy() && delay == 0)
	{
		camera->set_fuzzy(false);
		camera->render();
	}

	if (delay > 0)
		delay--;
	
	glutPostRedisplay();

	theta_x += v_x * d_theta;
	theta_y += v_y * d_theta;
	
	if (theta_y < 0)
		theta_y += 360.0;
	if (theta_y > 360.0)
		theta_y -= 360.0;
	if (theta_x < 0)
		theta_x += 360.0;
	if (theta_x > 360.0)
		theta_x -= 360.0;
}

int main(int argc, char **argv)
{
	FILE *window_fp;
	if ((window_fp = fopen("window.txt", "r")) == NULL)
	{
		view_w = 256;
		view_h = 256;
	}
	else
	{
		char buf[80];
		float val;
		while (fscanf(window_fp, "%s %f", buf, &val) == 2)
		{
			if (strcmp(buf, "width") == 0)
				view_w = val;
			else if (strcmp(buf, "height") == 0)
				view_h = val;
		}
	}
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(view_w, view_h);
	win_id = glutCreateWindow("Shader");

	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutKeyboardFunc(key_func);
	glutSpecialFunc(special_func);
	glutMotionFunc(motion_func);
	glutMouseFunc(mouse_func);

	init();

	glutMainLoop();

	return 0;
}
