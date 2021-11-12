#ifndef _COLLIDER_COMPONENT_H
#define _COLLIDER_COMPONENT_H

#include "collider.h"
#include <optional>

class ColliderComponent final : public Component
{
public:

	ColliderComponent() = default;

	ColliderComponent(GameEnums::Entities::DamagableType damage_type) : pCollider(nullptr), m_lives(std::nullopt), m_damagable_type(damage_type) { }

	ColliderComponent(GameEnums::Entities::DamagableType damage_type, int lives_number) : pCollider (nullptr),	m_lives(lives_number), m_damagable_type(damage_type) { }

	~ColliderComponent() override
	{
	}

	std::shared_ptr<Collider> pCollider;
	std::optional<int> m_lives = std::nullopt;
	GameEnums::Entities::DamagableType m_damagable_type = GameEnums::Entities::DamagableType::NONE;

private:
};

#endif
