#ifndef _UI_COMPONENT_H
#define _UI_COMPONENT_H

#include "ECS.h"
#include "game.h"
#include <string>

class UIComponent final : public Component
{
public:

	UIComponent(std::string text, glm::vec2 position, glm::vec3 color, float _scale, bool _align_center) : m_position(position),
		m_label_text(text), m_text_color(color), m_scale(_scale), m_is_align_center(_align_center) { }

	~UIComponent() override
	{
	}

	void draw() override
	{
		Game::text_renderer->render_text(m_label_text, m_position.x, m_position.y, m_scale, m_is_align_center, m_text_color);
	}

private:
	glm::vec2 m_position = { 0.0f, 0.0f };
	std::string m_label_text;
	glm::vec3 m_text_color = { 0.0f, 0.0f, 0.0f };
	float m_scale = 0.0f;
	bool m_is_align_center = true;
};

#endif
