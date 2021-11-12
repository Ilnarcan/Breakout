#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "window_manager.h"
#include "menu_state.h"


// Callback functions
void framebuffer_size_callback(GLFWwindow* glfw_window, int width, int height);
void key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mode);
void char_callback(GLFWwindow* glfw_window, unsigned int key);


GLFWwindow* WindowManager::m_window;

int WindowManager::init(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, false);

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(m_window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glfwSetCharCallback(m_window, char_callback);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	glViewport(0, 0, width, height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void WindowManager::terminate()
{
	glfwSetWindowShouldClose(m_window, true);
}


void WindowManager::update(float dt)
{
	state_manager->update(dt);
}

WindowManager::WindowManager()
{
	state_manager = new StateManager();
}

WindowManager* WindowManager::GetInstance()
{
	static WindowManager instance;
	return &instance;
}


void WindowManager::on_key(float dt)
{
	if (this->m_keys[GLFW_KEY_SPACE])
	{
		state_manager->on_key(dt, GLFW_KEY_SPACE);
	}
	if (this->m_keys[GLFW_KEY_DOWN] && !this->m_keys_processed[GLFW_KEY_DOWN])
	{
		this->m_keys_processed[GLFW_KEY_DOWN] = true;
		state_manager->on_key(dt, GLFW_KEY_DOWN);
	}
	if (this->m_keys[GLFW_KEY_UP] && !this->m_keys_processed[GLFW_KEY_UP])
	{
		this->m_keys_processed[GLFW_KEY_UP] = true;
		state_manager->on_key(dt, GLFW_KEY_UP);
	}
	if (this->m_keys[GLFW_KEY_ENTER] && !this->m_keys_processed[GLFW_KEY_ENTER])
	{
		this->m_keys_processed[GLFW_KEY_ENTER] = true;
		state_manager->on_key(dt, GLFW_KEY_ENTER);
	}
	if (this->m_keys[GLFW_KEY_BACKSPACE] && !this->m_keys_processed[GLFW_KEY_BACKSPACE])
	{
		this->m_keys_processed[GLFW_KEY_BACKSPACE] = true;
		state_manager->on_key(dt, GLFW_KEY_BACKSPACE);
	}
	if (this->m_keys[GLFW_KEY_ESCAPE] && !this->m_keys_processed[GLFW_KEY_ESCAPE])
	{
		this->m_keys_processed[GLFW_KEY_ESCAPE] = true;
		state_manager->on_key(dt, GLFW_KEY_ESCAPE);
	}
	if (this->m_keys[GLFW_KEY_A])
	{
		state_manager->on_key(dt, GLFW_KEY_A);
	}
	if (this->m_keys[GLFW_KEY_D])
	{
		state_manager->on_key(dt, GLFW_KEY_D);
	}
}

void WindowManager::on_char(unsigned int key)
{
	state_manager->on_char(key);
}

void WindowManager::draw()
{
	state_manager->draw();
}


void WindowManager::change_state()
{
	state_manager->change_state(MenuState::GetInstance(state_manager));
}

// Callback functions

void key_callback(GLFWwindow* glfw_window, int key, int scancode, int action, int mode)
{
	WindowManager* window = WindowManager::GetInstance();

	if (window)
	{
		if (key >= 0 && key < 1024)
		{
			if (action == GLFW_PRESS)
				window->m_keys[key] = true;
			else if (action == GLFW_RELEASE)
			{
				window->m_keys[key] = false;
				window->m_keys_processed[key] = false;
			}
		}
	}
}

void char_callback(GLFWwindow* glfw_window, unsigned int key)
{
	WindowManager* window = WindowManager::GetInstance();
	if (window)
	{
		window->on_char(key);
	}
}

void framebuffer_size_callback(GLFWwindow* glfw_window, int width, int height)
{
	glViewport(0, 0, width, height);
}
