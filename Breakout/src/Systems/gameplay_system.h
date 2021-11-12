#ifndef _GAMEPLAY_SYSTEM_H
#define _GAMEPLAY_SYSTEM_H

#include "game.h"
#include "ECS.h"
#include "components.h"

extern Manager manager;

class GameplaySystem
{
public:
	static void update(GameEnums::GameState& game_state);

};

#endif
