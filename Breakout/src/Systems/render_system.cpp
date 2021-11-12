
#include "render_system.h"

void RenderSystem::draw()
{
	draw(manager.get_group(GameEnums::Entities::GroupType::IMG));
	draw(manager.get_group(GameEnums::Entities::GroupType::LABEL));

	draw(manager.get_group(GameEnums::Entities::GroupType::BRICK));
	draw(manager.get_group(GameEnums::Entities::GroupType::PLAYER));
	draw(manager.get_group(GameEnums::Entities::GroupType::BALL));
}

void RenderSystem::draw(const std::vector<Entity*>& entities)
{
	for (auto& e : entities)
	{
		e->draw();
	}
}
