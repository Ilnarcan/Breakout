#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "game_enums.h"
#include "text_renderer.h"
#include "sprite_renderer.h"

extern const float SCREEN_WIDTH;
extern const float SCREEN_HEIGHT;

class StateManager;

class GameState
{
public:
	GameState(StateManager* manager);
	virtual ~GameState() = default;

	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual void on_key(float dt, int key) = 0;
	virtual void on_char(unsigned int key) = 0;

	virtual void enter_state()
	{
	}

	virtual void leave_state()
	{
	}


protected:
	void change_state(GameState* new_state) const;

	StateManager* stateManager;
	GameEnums::GameState game_state;
};

#endif
