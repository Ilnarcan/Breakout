#include "game_state.h"
#include "state_manager.h"

GameState::GameState(StateManager* manager)
	: stateManager(manager)
{
}

void GameState::change_state(GameState* new_state) const
{
	stateManager->change_state(new_state);
}
