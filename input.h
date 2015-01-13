#ifndef __INPUT_H
#define __INPUT_H

#include "common.h"
#include "system.h"

typedef enum
{
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_SHOOT,
	BUTTON_JUMP,
	BUTTON_SELECT,
	BUTTON_PAUSE,
	BUTTON_L_SPACE,
	BUTTON_R_SPACE,
	BUTTON_L_GRAV,
	BUTTON_R_GRAV
} Button;

class InputDevice;

class InputListener
{
public:
	InputListener(InputDevice *device);
	virtual ~InputListener();
	
	virtual void button_pressed(Button button);
	virtual void button_released(Button button);

	void set_input(InputDevice *device);
protected:
	InputDevice *m_input;
};

class InputDevice
{
public:
	virtual ~InputDevice() {}
	
	virtual bool pressed(Button button) = 0;

	void add_input_listener(InputListener *listener);
	void remove_input_listener(InputListener *listener);

	void move_listeners(InputDevice *device);
protected:
	void synchronize();
	
	set<InputListener *> m_listeners;
	set<InputListener *> m_listeners_add;
};

class KeyboardInput : public InputDevice, KeyListener
{
public:
	KeyboardInput();
	~KeyboardInput();

	bool pressed(Button button);
	void key_event(SDL_KeyboardEvent event);

	void set_button(Button button, SDLKey key);
private:
	map<Button, SDLKey> m_keys;
	map<Button, bool> m_pressed;
};

#endif
