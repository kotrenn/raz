#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "common.h"
#include "singletonmanager.h"
#include "surface.h"

class KeyListener;
class KeyManager;
class MouseListener;
class MouseManager;

class System
{
public:
	static System *get_instance();
	~System();

	bool run();
	void flip();

	Surface *get_screen() const { return m_screen; }
	void get_size(int *w, int *h) const;
	
	void add_key_listener(KeyListener *listener);
	void add_mouse_listener(MouseListener *listener);
	
	void remove_key_listener(KeyListener *listener);
	void remove_mouse_listener(MouseListener *listener);

	void exit() { m_exit = true; }
private:
	System();
	static System *m_instance;

	void init_SDL();
	void exit_SDL();

	SingletonManager *m_sm;

	KeyManager *m_key_manager;
	MouseManager *m_mouse_manager;

	bool m_exit;

	/* SDL stuff */
	int m_width;
	int m_height;
	int m_size[2];
	bool m_fullscreen;

	int m_display_flags;
	SDL_Surface *m_sdl_screen;
	Surface *m_screen;

	int m_frame;
	Uint32 m_total;
};

class KeyListener
{
public:
	virtual void key_event(SDL_KeyboardEvent event);
	virtual void key_pressed(SDLKey key, SDLMod mod);
	virtual void key_released(SDLKey key, SDLMod mod);
};

class KeyManager
{
public:
	void event(SDL_Event event) const;
	
	void add_key_listener(KeyListener *listener);
	void remove_key_listener(KeyListener *listener);
private:
	set<KeyListener *> m_listeners;
};

class MouseListener
{
public:
	virtual void mouse_motion(SDL_MouseMotionEvent event);
	virtual void mouse_down(SDL_MouseButtonEvent event);
	virtual void mouse_up(SDL_MouseButtonEvent event);
};

class MouseManager
{
public:
	void event(SDL_Event event) const;

	void add_mouse_listener(MouseListener *listener);
	void remove_mouse_listener(MouseListener *listener);
private:
	set<MouseListener *> m_listeners;
};

#endif
