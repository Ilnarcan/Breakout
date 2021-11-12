#ifndef _SPRITE_COMPONENT_H
#define _SPRITE_COMPONENT_H

#include "game.h"
#include "components.h"

class SpriteComponent final : public Component
{
public:
	SpriteComponent() = default;

	SpriteComponent(std::string id) : transform(nullptr)
	{
		texture = Game::assets->get_texture(std::move(id));
	}

	~SpriteComponent() override
	{
	}

	void init() override
	{
		transform = &entity->get_component<TransformComponent>();
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
