#ifndef _BOX_COLLIDER_H
#define _BOX_COLLIDER_H

#include <glm/vec2.hpp>
#include "collider.h"

class BoxCollider final : public Collider
{
public:
	BoxCollider(glm::vec2 _size, bool trigger) : Collider(trigger), size(_size)
	{
	}

	~BoxCollider() override
	{
	}

	glm::vec2 get_size() const { return size; }

private:
	glm::vec2 size;
};

#endif
