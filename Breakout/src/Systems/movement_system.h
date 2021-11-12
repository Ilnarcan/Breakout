#ifndef _MOVEMENT_SYSTEM_H
#define _MOVEMENT_SYSTEM_H

#include <set>

#include "game.h"
#include "ECS.h"
#include "components.h"
#include "game_enums.h"

extern Manager manager;

class MovementSystem
{
public:
	static void move(glm::vec2 velocity, float dt, int key);

	static void update(float dt);
};

#endif
