
#include "movement_system.h"

void MovementSystem::move(glm::vec2 velocity, float dt, int key)
{
	const auto& entities = manager.get_group(GameEnums::Entities::GroupType::PLAYER);
	for (const auto entity : entities)
	{
		if (entity->has_component<TransformComponent>())
		{
			TransformComponent* transform = &entity->get_component<TransformComponent>();

			if (entity->has_component<SpriteComponent>())
			{
				transform->m_velocity = velocity;
			}
		}
	}
}

void MovementSystem::update(float dt)
{
	const auto& player_entities = manager.get_group(GameEnums::Entities::GroupType::PLAYER);
	const auto& ball_entities = manager.get_group(GameEnums::Entities::GroupType::BALL);

	std::set<Entity*> entities;

	auto insert_to_set = [&entities](const std::vector<Entity*>& numbers)
	{
		std::for_each(numbers.begin(), numbers.end(), [&entities](const auto& elem)
			{
				entities.insert(elem);
			});
	};

	insert_to_set(player_entities);
	insert_to_set(ball_entities);


	for (const auto entity : entities)
	{
		if (entity->has_component<TransformComponent>())
		{
			TransformComponent* transform = &entity->get_component<TransformComponent>();

			transform->m_position.x += transform->m_velocity.x * dt;
			transform->m_position.y += transform->m_velocity.y * dt;


			if (entity->has_component<SpriteComponent>())
			{
				if (transform->m_position.x < 0.0f)
				{
					transform->m_position.x = 0.0f;
				}

				if (transform->m_position.x > SCREEN_WIDTH - transform->m_size.x)
				{
					transform->m_position.x = SCREEN_WIDTH - transform->m_size.x;
				}

				transform->m_velocity = { 0.0f, 0.0f };
			}
			if (entity->has_component<BallComponent>())
			{
				if (transform->m_position.x <= 0.0f)
				{
					transform->m_velocity.x = -transform->m_velocity.x;
					transform->m_position.x = 0.0f;
				}
				else if (transform->m_position.x + transform->m_size.x >= SCREEN_WIDTH)
				{
					transform->m_velocity.x = -transform->m_velocity.x;
					transform->m_position.x = SCREEN_WIDTH - transform->m_size.x;
				}
				if (transform->m_position.y <= 0.0f)
				{
					transform->m_velocity.y = -transform->m_velocity.y;
					transform->m_position.y = 0.0f;
				}
			}
		}
	}
}