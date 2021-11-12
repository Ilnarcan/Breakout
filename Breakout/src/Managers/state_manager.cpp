#include "game_state.h"
#include "state_manager.h"

StateManager::StateManager()
	: active_state(nullptr)
{
}

StateManager::~StateManager()
{
}

void StateManager::change_state(GameState* new_state)
{
	if (active_state)
		active_state->leave_state();
	active_state = new_state;
	active_state->enter_state();
}

void StateManager::update(float dt) const
{
	if (active_state)
	{
		active_state->update(dt);
	}
}

void StateManager::draw() const
{
	if (active_state)
	{
		active_state->draw();
	}
}

void StateManager::on_key(float dt, int key) const
{
	if (active_state)
	{
		active_state->on_key(dt, key);
	}
}

void StateManager::on_char(unsigned int key) const
{
	if (active_state)
	{
		active_state->on_char(key);
	}
}
