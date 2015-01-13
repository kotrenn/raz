#include "manager.h"

#include "datamanager.h"
#include "resourcemanager.h"
#include "system.h"

Manager::Manager()
	:m_prev_move(SDL_GetTicks()),
	 m_pause_ticks(0),
	 m_paused(false),
	 m_key_device(NULL),
	 m_joy_device(NULL),
	 m_input_device(NULL)
{
	System *system = System::get_instance();
	system->add_key_listener(this);
	system->add_mouse_listener(this);

	m_key_device = new KeyboardInput();
	//m_joy_device = new JoystickInput();
	m_input_device = m_key_device;

	/* put in scene object creation here */
	
	/* Initialize OpenGL */
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { -1.0, -1.0, -1.0, 0.0 };
	GLfloat white_light[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat lmodel_ambient[] = { 0.5, 0.5, 0.5, 1.0 };

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_prev_move = SDL_GetTicks();
}

Manager::~Manager()
{
	System *system = System::get_instance();
	system->remove_key_listener(this);
	system->remove_mouse_listener(this);

	delete m_key_device;
	//delete m_joy_device;
}

void Manager::key_pressed(SDLKey key, SDLMod)
{
	if (key == SDLK_p)
	{
		if (m_paused)
			unpause();
		else
			pause();
	}
}

void Manager::key_released(SDLKey, SDLMod)
{
}

void Manager::mouse_down(SDL_MouseButtonEvent)
{
}

void Manager::pause()
{
	m_paused = true;
	m_pause_ticks = SDL_GetTicks() - m_prev_move;
}

void Manager::unpause()
{
	m_paused = false;
	m_prev_move = SDL_GetTicks() - m_pause_ticks;
	m_pause_ticks = 0;
}

void Manager::update()
{
	if (m_paused)
		return;

	Uint32 cur_time = SDL_GetTicks();
	double dt = (cur_time - m_prev_move) / 1000.0;
	//dt = min(dt, 0.05);
	m_prev_move = cur_time;

	// update scene by dt
}

void Manager::draw(Surface *dst)
{
	/* Init OpenGL viewing */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	System *system = System::get_instance();
	int width, height;
	system->get_size(&width, &height);
	glOrtho(0.0, width, height, 0, -100, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// draw scene here
}
