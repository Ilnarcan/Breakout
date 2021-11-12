#include "ECS.h"


void Entity::draw()
{
	for (auto& c : components)
	{
		c->draw();
	}
}

bool Entity::is_active() const
{
	return active;
}
void Entity::destroy()
{
	active = false;
}


bool Entity::has_group(Group mGroup)
{
	return groupBitset[mGroup];
}

void Entity::add_group(const Group mGroup)
{
	groupBitset[mGroup] = true;
	manager.add_to_group(this, mGroup);
}

void Entity::del_group(Group mGroup)
{
	groupBitset[mGroup] = false;
}


void Manager::refresh()
{
	for (size_t i(0u); i < maxGroups; i++)
	{
		auto& v(groupedEntities[i]);
		v.erase(
			std::remove_if(std::begin(v), std::end(v),
				[i](Entity* mEntity)
				{
					return !mEntity->is_active() || !mEntity->has_group(i);
				}),
			std::end(v));
	}

	entities.erase(std::remove_if(std::begin(entities), std::end(entities),
		[](const std::unique_ptr<Entity>& mEntity)
		{
			return !mEntity->is_active();
		}),
		std::end(entities));
}

void Manager::reset()
{
	for (size_t i(0u); i < maxGroups; i++)
	{
		auto& v(groupedEntities[i]);
		v.clear();
	}

	entities.clear();
}

void Manager::add_to_group(Entity* mEntity, Group mGroup)
{
	groupedEntities[mGroup].emplace_back(mEntity);
}

std::vector<Entity*>& Manager::get_group(Group mGroup)
{
	return groupedEntities[mGroup];
}

Entity& Manager::add_entity()
{
	auto e = new Entity(*this);
	std::unique_ptr<Entity> uPtr{ e };
	entities.emplace_back(std::move(uPtr));
	return *e;
}
