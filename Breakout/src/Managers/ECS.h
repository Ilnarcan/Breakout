#ifndef _ECS_H
#define _ECS_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include "game_enums.h"

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID get_new_component_type_id()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T>
ComponentID get_component_type_id() noexcept
{
	static_assert (std::is_base_of_v<Component, T>, "");
	static ComponentID typeID = get_new_component_type_id();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

using ComponentArray = std::array<Component*, maxComponents>;

using GroupType = GameEnums::Entities::GroupType;

class Component
{
public:
	Entity* entity;

	virtual void init()
	{
	}

	virtual void update()
	{
	}

	virtual void draw()
	{
	}

	virtual ~Component()
	{
	}
};

class Entity
{
public:
	Entity(Manager& mManager) : manager(mManager)
	{
	}

	void draw();

	bool is_active() const;
	void destroy();

	bool has_group(Group mGroup);
	void add_group(Group mGroup);
	void del_group(Group mGroup);

	template <typename T>
	bool has_component() const
	{
		return componentBitset[get_component_type_id<T>()];
	}

	template <typename T, typename... TArgs>
	T& add_component(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{c};
		components.emplace_back(std::move(uPtr));

		componentArray[get_component_type_id<T>()] = c;
		componentBitset.set(get_component_type_id<T>());

		c->init();
		return *c;
	}

	template <typename T>
	T& get_component() const
	{
		auto ptr(componentArray[get_component_type_id<T>()]);
		return *static_cast<T*>(ptr);
	}

private:
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
	GroupBitset groupBitset;

};

class Manager
{
public:

	void refresh();
	void reset();

	void add_to_group(Entity* mEntity, Group mGroup);
	std::vector<Entity*>& get_group(Group mGroup);

	Entity& add_entity();

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

};

#endif
