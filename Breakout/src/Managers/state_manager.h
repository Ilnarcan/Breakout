#ifndef _STATE_MANAGER_H
#define _STATE_MANAGER_H

#include "game_state.h"

class StateManager
{
public:
	StateManager();
	~StateManager();

	void change_state(GameState* new_state);

	void update(float dt) const;
	void draw() const;
	void on_key(float dt, int key) const;
	void on_char(unsigned int key) const;

private:
	GameState* active_state;
};

#endif
