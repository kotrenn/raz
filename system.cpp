#include "system.h"

#include "resourcemanager.h"

System *System::m_instance = NULL;

System *System::get_instance()
{
	if (m_instance == NULL)
		m_instance = new System();
	return m_instance;
}

System::System()
	:m_sm(NULL),
	 m_key_manager(NULL),
	 m_mouse_manager(NULL),
	 m_exit(false)
{
	srand(time(0));
	
	init_SDL();
	
	m_sm = new SingletonManager();
	
	m_key_manager = new KeyManager();
	m_mouse_manager = new MouseManager();
}

System::~System()
{
	delete m_sm;
	
	delete m_key_manager;
	delete m_mouse_manager;

	exit_SDL();
}

void System::init_SDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_EnableUNICODE(1);
	/*
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
	                    SDL_DEFAULT_REPEAT_INTERVAL);
	*/

	m_width = 640;
	m_height = 480;
	m_size[0] = m_width;
	m_size[1] = m_height;

	m_display_flags = SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_DOUBLEBUF | SDL_OPENGL;
	m_sdl_screen = SDL_SetVideoMode(m_size[0], m_size[1],
	                                32, m_display_flags);
	m_screen = new Surface(m_sdl_screen);
	m_sdl_screen->refcount--;

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		fprintf(stderr, "Error initializing OpenGL: %s\n",
		        gluErrorString(error));
		fflush(stderr);
		//exit(1);
	}

	SDL_WM_SetCaption("Raz Visualizer", 0);

	m_frame = 0;
	m_total = SDL_GetTicks();
}

void System::exit_SDL()
{
	delete m_screen;
	
	SDL_FreeSurface(m_sdl_screen);

	SDL_Quit();
}

bool System::run()
{
	bool ret = true;
	
	if (SDL_GetTicks() - m_total >= 1000)
	{
		m_total = SDL_GetTicks();
		m_frame = 0;
	}

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			ret = false;
		else if (event.type == SDL_KEYDOWN)
		{
			SDLKey sym = event.key.keysym.sym;
			SDLMod mod = event.key.keysym.mod;
			if (sym == SDLK_ESCAPE)
				ret = false;
			else if (sym == SDLK_RETURN &&
			         (mod & (KMOD_LALT |
			                 KMOD_RALT)))
			{
				int flags = m_display_flags;
				m_fullscreen = !m_fullscreen;
				if (m_fullscreen)
					flags |= SDL_FULLSCREEN;
				delete m_screen;
				SDL_FreeSurface(m_sdl_screen);
				m_sdl_screen = SDL_SetVideoMode(m_size[0],
				                                m_size[1],
				                                32, flags);
				m_screen = new Surface(m_sdl_screen);
			}

			m_key_manager->event(event);
		}
		else if (event.type == SDL_KEYUP)
			m_key_manager->event(event);
		else if (event.type == SDL_MOUSEMOTION)
			m_mouse_manager->event(event);
		else if (event.type == SDL_MOUSEBUTTONDOWN)
			m_mouse_manager->event(event);
		else if (event.type == SDL_MOUSEBUTTONUP)
			m_mouse_manager->event(event);
	}

	if (m_exit)
		ret = false;

	return ret;
}

void System::flip()
{
	ResourceManager *rm = ResourceManager::get_instance();
	Font *font = rm->get_font();

	char fps_string[81];
	float fps = m_frame / ((SDL_GetTicks() - m_total) / 1000.0);
	sprintf(fps_string, "fps: %f", fps);
	font->draw_string(m_screen, 5, 455, fps_string);
	m_frame++;

	SDL_GL_SwapBuffers();
	
	//m_screen->flip();
}

void System::get_size(int *w, int *h) const
{
	*w = m_screen->get_w();
	*h = m_screen->get_h();
}

void System::add_key_listener(KeyListener *listener)
{
	m_key_manager->add_key_listener(listener);
}

void System::add_mouse_listener(MouseListener *listener)
{
	m_mouse_manager->add_mouse_listener(listener);
}

void System::remove_key_listener(KeyListener *listener)
{
	m_key_manager->remove_key_listener(listener);
}

void System::remove_mouse_listener(MouseListener *listener)
{
	m_mouse_manager->remove_mouse_listener(listener);
}



void KeyListener::key_event(SDL_KeyboardEvent) {}
void KeyListener::key_pressed(SDLKey, SDLMod) {}
void KeyListener::key_released(SDLKey, SDLMod) {}

void KeyManager::event(SDL_Event event) const
{
	SDLKey sym = event.key.keysym.sym;
	SDLMod mod = event.key.keysym.mod;

	for (auto it : m_listeners)
		it->key_event(event.key);

	if (event.type == SDL_KEYDOWN)
		for (auto it : m_listeners)
			it->key_pressed(sym, mod);

	if (event.type == SDL_KEYUP)
		for (auto it : m_listeners)
			it->key_released(sym, mod);
}

void KeyManager::add_key_listener(KeyListener *listener)
{
	m_listeners.insert(listener);
}

void KeyManager::remove_key_listener(KeyListener *listener)
{
	m_listeners.erase(listener);
}



void MouseListener::mouse_motion(SDL_MouseMotionEvent) {}
void MouseListener::mouse_down(SDL_MouseButtonEvent) {}
void MouseListener::mouse_up(SDL_MouseButtonEvent) {}

void MouseManager::event(SDL_Event event) const
{
	if (event.type == SDL_MOUSEMOTION)
		for (auto it : m_listeners)
			it->mouse_motion(event.motion);

	if (event.type == SDL_MOUSEBUTTONDOWN)
		for (auto it : m_listeners)
			it->mouse_down(event.button);

	if (event.type == SDL_MOUSEBUTTONUP)
		for (auto it : m_listeners)
			it->mouse_up(event.button);
}

void MouseManager::add_mouse_listener(MouseListener *listener)
{
	m_listeners.insert(listener);
}

void MouseManager::remove_mouse_listener(MouseListener *listener)
{
	m_listeners.erase(listener);
}
