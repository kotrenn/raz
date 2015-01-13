#ifndef __MANAGER_H
#define __MANAGER_H

#include "camera.h"
#include "input.h"

class Manager : public KeyListener, MouseListener
{
public:
	Manager();
	~Manager();

	void key_pressed(SDLKey key, SDLMod mod);
	void key_released(SDLKey key, SDLMod mod);
	void mouse_down(SDL_MouseButtonEvent event);

	void pause();
	void unpause();

	void update();
	void draw(Surface *dst);
private:
	Uint32 m_prev_move;
	Uint32 m_pause_ticks;
	bool m_paused;

	InputDevice *m_key_device;
	InputDevice *m_joy_device;
	InputDevice *m_input_device;
};

#endif
