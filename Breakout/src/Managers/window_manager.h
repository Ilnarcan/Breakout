#ifndef _WINDOW_MANAGER_H_
#define _WINDOW_MANAGER_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "state_manager.h"

class StateManager;

class WindowManager
{
public:
	WindowManager();

	static WindowManager* GetInstance();

	static int init(int width, int height, const char* title);
	static void terminate();

	void update(float dt);
	void on_key(float dt);
	void on_char(unsigned int key);
	void draw();
	void change_state();


	static GLFWwindow* m_window;

	bool m_keys[1024];
	bool m_keys_processed[1024];

private:
	StateManager* state_manager;
};


#endif
