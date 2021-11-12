
#include "collision_system.h"

void CollisionSystem::update(float dt, int& score)
{
	const auto paddle = manager.get_group(GameEnums::Entities::GroupType::PLAYER)[0];
	const ColliderComponent* paddle_collider = &paddle->get_component<ColliderComponent>();
	const TransformComponent* paddle_transform = &paddle->get_component<TransformComponent>();


	const auto ball = manager.get_group(GameEnums::Entities::GroupType::BALL)[0];
	const ColliderComponent* ball_collider = &ball->get_component<ColliderComponent>();
	TransformComponent* ball_transform = &ball->get_component<TransformComponent>();


	const auto& bricks = manager.get_group(GameEnums::Entities::GroupType::BRICK);

	// bricks - ball

	for (const auto brick : bricks)
	{
		if (brick->has_component<TransformComponent>())
		{
			const TransformComponent* brick_transform = &brick->get_component<TransformComponent>();

			if (brick->has_component<ColliderComponent>())
			{
				ColliderComponent* brick_collider = &brick->get_component<ColliderComponent>();

				Collision collision = check_collision(ball_transform, brick_transform,
					std::dynamic_pointer_cast<SphereCollider>(
						ball_collider->pCollider),
					std::dynamic_pointer_cast<BoxCollider>(
						brick_collider->pCollider));

				if (std::get<0>(collision))
				{
					if (brick_collider->m_damagable_type == GameEnums::Entities::DamagableType::DESTROYABLE)
					{
						brick_collider->m_lives = brick_collider->m_lives.value() - 1;
						if (brick_collider->m_lives.value() <= 0)
						{
							brick->destroy();
							score++;
						}
						else
						{
							StaticComponent* brick_static = &brick->get_component<StaticComponent>();
							brick_static->set_color(brick_collider->m_lives.value());
						}
					}

					const DIRECTION dir = std::get<1>(collision);
					const glm::vec2 diff_vector = std::get<2>(collision);

					std::shared_ptr<SphereCollider> ball_collider_shared = std::dynamic_pointer_cast<
						SphereCollider>(ball_collider->pCollider);
					SphereCollider* ball_sphere = ball_collider_shared.get();

					if (dir == DIRECTION::LEFT || dir == DIRECTION::RIGHT)
					{
						ball_transform->m_velocity.x = -ball_transform->m_velocity.x;

						const float penetration = ball_sphere->get_radius() - std::abs(diff_vector.x);
						if (dir == DIRECTION::LEFT)
						{
							ball_transform->m_position.x += penetration;
						}
						else
						{
							ball_transform->m_position.x -= penetration;
						}
					}
					else
					{
						ball_transform->m_velocity.y = -ball_transform->m_velocity.y;

						const float penetration = ball_sphere->get_radius() - std::abs(diff_vector.y);
						if (dir == DIRECTION::UP)
						{
							ball_transform->m_position.y -= penetration;
						}
						else
						{
							ball_transform->m_position.y += penetration;
						}
					}
				}
			}
		}
	}

	// paddle - ball

	const Collision collision = check_collision(ball_transform, paddle_transform,
		std::dynamic_pointer_cast<SphereCollider>(ball_collider->pCollider),
		std::dynamic_pointer_cast<BoxCollider>(paddle_collider->pCollider));

	if (std::get<0>(collision))
	{
		std::shared_ptr<SphereCollider> ball_collider_shared = std::dynamic_pointer_cast<SphereCollider>(
			ball_collider->pCollider);
		SphereCollider* ball_sphere = ball_collider_shared.get();

		const float centerBoard = paddle_transform->m_position.x + paddle_transform->m_size.x / 2.0f;
		const float distance = (ball_transform->m_position.x + ball_sphere->get_radius()) - centerBoard;
		const float percentage = distance / (paddle_transform->m_size.x / 2.0f);

		const float strength = 2.0f;
		const glm::vec2 oldVelocity = ball_transform->m_velocity;
		ball_transform->m_velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		ball_transform->m_velocity.y = -1.0f * abs(ball_transform->m_velocity.y);
		ball_transform->m_velocity = glm::normalize(ball_transform->m_velocity) * length(oldVelocity);
	}
}

Collision CollisionSystem::check_collision(const TransformComponent* first, const TransformComponent* second,
	const std::shared_ptr<SphereCollider>& first_collider,
	const std::shared_ptr<BoxCollider>& second_collider)
{
	const glm::vec2 center(first->m_position + first_collider->get_radius());

	const glm::vec2 aabb_half_extents(second_collider->get_size().x / 2.0f, second_collider->get_size().y / 2.0f);
	const glm::vec2 aabb_center(second->m_position.x + aabb_half_extents.x, second->m_position.y + aabb_half_extents.y);

	glm::vec2 difference = center - aabb_center;
	const glm::vec2 clamped = clamp(difference, -aabb_half_extents, aabb_half_extents);

	const glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;

	auto len = length(difference);

	if (length(difference) < first_collider->get_radius()) {
		return std::make_tuple(true, vector_direction(difference), difference);
	}
	return std::make_tuple(false, DIRECTION::UP, glm::vec2(0.0f, 0.0f));
}

DIRECTION CollisionSystem::vector_direction(const glm::vec2 target)
{
	const glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		const float dot_product = dot(normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return static_cast<DIRECTION>(best_match);
}
