#ifndef _STATIC_COMPONENT_H
#define _STATIC_COMPONENT_H

#include "asset_manager.h"
#include "game.h"

constexpr int COLORS_NUMBER = 5;

inline std::array<glm::vec3, COLORS_NUMBER> colors =
{
	glm::vec3{1.0f, 1.0f, 1.0f}, // 0
	glm::vec3{0.2f, 0.6f, 1.0f}, // 1
	glm::vec3{0.0f, 0.7f, 0.0f}, // 2
	glm::vec3{0.8f, 0.8f, 0.4f}, // 3
	glm::vec3{1.0f, 0.5f, 0.0f}, // 4
};


class StaticComponent final : public Component
{

public:
	StaticComponent() = default;

	StaticComponent(std::string id, glm::vec2 _position, glm::vec2 _size, glm::vec3 _color) : m_position(_position),
		m_size(_size), m_color(_color)
	{
		texture = Game::assets->get_texture(std::move(id));
	}

	StaticComponent(std::string id, glm::vec2 _position, glm::vec2 _size, int color_index) : m_position(_position),
		m_size(_size), m_color(colors[color_index])
	{
		texture = Game::assets->get_texture(std::move(id));
	}

	StaticComponent(std::string id, glm::vec2 _position, glm::vec2 _size) : m_position(_position), m_size(_size), m_color(std::nullopt)
	{
		texture = Game::assets->get_texture(std::move(id));
	}

	~StaticComponent() override
	{
	}

	void draw() override
	{
		if (m_color.has_value())
			Game::sprite_renderer->draw_sprite(texture, m_position, m_size, 0.0f, m_color.value());
		else
			Game::sprite_renderer->draw_sprite(texture, m_position, m_size);
	}

	void set_color(int color_index)
	{
		m_color = colors[color_index];
	}

private:
	Texture2D* texture = nullptr;

	glm::vec2 m_position = { 0.0f, 0.0f };
	glm::vec2 m_size = { 0.0f, 0.0f };
	std::optional<glm::vec3> m_color = std::nullopt;

};

#endif
