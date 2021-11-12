#ifndef _TRASNFORM_COMPONENT_H
#define _TRANSFORM_COMPONENT_H

#include <glm/vec2.hpp>

#include "ECS.h"

class TransformComponent final : public Component
{
public:

	TransformComponent() = default;

	TransformComponent(glm::vec2 position, glm::vec2 size) : m_position(position), m_size(size) { }

	TransformComponent(glm::vec2 position, glm::vec2 size, glm::vec2 velocity) : m_position(position), m_size(size), m_velocity(velocity) { }

	glm::vec2 m_position = {0.0f, 0.0f};
	glm::vec2 m_size = { 0.0f, 0.0f };
	glm::vec2 m_velocity = { 0.0f, 0.0f };

};

#endif