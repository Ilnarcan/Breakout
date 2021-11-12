#ifndef _BALL_COMPONENT_H
#define _BALL_COMPONENT_H

#include "game.h"
#include "ECS.h"

class BallComponent final : public Component
{
public:

	BallComponent() = default;

	BallComponent(std::string id)
	{
		texture = Game::assets->get_texture(std::move(id));
	}

	~BallComponent() override
	{
	}

	void init() override
	{
		transform = &entity->get_component<TransformComponent>();
	}

	void update() override
	{
	}

	void draw() override
	{
		Game::sprite_renderer->draw_sprite(texture, transform->m_position, transform->m_size);
	}

private:
	TransformComponent* transform = nullptr;
	Texture2D* texture = nullptr;

};

#endif
