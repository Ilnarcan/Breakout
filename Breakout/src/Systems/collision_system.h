#ifndef _COLLISION_SYSTEM_H
#define _COLLISION_SYSTEM_H

#include "box_collider.h"
#include "game.h"
#include "ECS.h"
#include "components.h"
#include "game_enums.h"
#include "sphere_collider.h"

extern const glm::vec2 INITIAL_BALL_VELOCITY;

extern Manager manager;

enum class DIRECTION
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

using Collision = std::tuple<bool, DIRECTION, glm::vec2>;


class CollisionSystem
{
public:
	static void update(float dt, int& score);

	static Collision check_collision(const TransformComponent* first, const TransformComponent* second,
		const std::shared_ptr<SphereCollider>& first_collider,
		const std::shared_ptr<BoxCollider>& second_collider);

	static DIRECTION vector_direction(const glm::vec2 target);

};

#endif
