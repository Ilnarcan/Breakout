
#include "gameplay_system.h"

void GameplaySystem::update(GameEnums::GameState& game_state)
{
	const auto& ball_entities = manager.get_group(GameEnums::Entities::GroupType::BALL);

	int alive_balls_count = ball_entities.size();

	for (const auto entity : ball_entities)
	{
		if (entity->has_component<TransformComponent>())
		{
			const TransformComponent* transform = &entity->get_component<TransformComponent>();

			if (transform->m_position.y > SCREEN_HEIGHT)
			{
				alive_balls_count--;
			}
		}
	}

	if (alive_balls_count < 1)
	{
		game_state = GameEnums::GameState::GAME_OVER;
		return;
	}

	const auto& brick_entities = manager.get_group(GameEnums::Entities::GroupType::BRICK);
	const int alive_destroyable_bricks = std::count_if(brick_entities.begin(), brick_entities.end(),
		[](const Entity* entity)
		{
			const auto& brick_collider = entity->get_component<
				ColliderComponent>();
			return brick_collider.m_damagable_type ==
				GameEnums::Entities::DamagableType::DESTROYABLE;
		});

	if (alive_destroyable_bricks == 0)
	{
		game_state = GameEnums::GameState::GAME_WIN;
	}
}
