#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "SDL/include/SDL_gamecontroller.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50
#define MAX_PADS 1

struct _SDL_GameController;
struct _SDL_Haptic;

struct SDL_Rect;

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};
struct GamePad
{
	//Input data
	bool start, back, guide;
	bool start_down, back_down, guide_down;
	bool _start_down, _back_down, _guide_down;
	bool x, y, a, b, l1, r1, l3, r3;
	bool _x_down, _y_down, _a_down, _b_down, _l1_down, _r1_down, _l3_down, _r3_down;
	bool x_down, y_down, a_down, b_down, l1_down, r1_down, l3_down, r3_down;
	bool up, down, left, right;
	bool _up_down, _down_down, _left_down, _right_down;
	bool up_down, down_down, left_down, right_down;
	float l2, r2;
	float l_x, l_y, r_x, r_y, l_dz, r_dz;
	bool x_idle, y_idle, a_idle, b_idle, l1_idle, r1_idle, l3_idle, r3_idle;
	//Controller data
	bool enabled;
	int index;
	_SDL_GameController* controller;
	_SDL_Haptic* haptic;

	//Rumble controller
	int rumble_countdown;
	float rumble_strength;

};
class Input : public Module
{

public:
	GamePad pads[MAX_PADS];
	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	void HandleDeviceConnection(int index);

	void HandleDeviceRemoval(int index);

	void UpdateGamepadsInput();

	bool ShakeController(int id, int duration, float strength);

	const char* GetControllerName(int id) const;

	// Check if a certain window event happened
	bool GetWindowEvent(EventWindow ev);

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	SDL_Joystick* joy = nullptr;
private:
	bool windowEvents[WE_COUNT];
	KeyState*	keyboard;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	int	mouseMotionX;
	int mouseMotionY;
	int mouseX;
	int mouseY;
};

#endif // __INPUT_H__