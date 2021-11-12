#include "game_level.h"
#include "game.h"
#include <fstream>
#include <random>
#include <sstream>

#include "box_collider.h"
#include "ECS.h"
#include "components.h"

extern Manager manager;

static auto rng = []
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	return rng;
}();

template <class T>
using uniform_distribution =
std::conditional_t<
	std::is_floating_point_v<T>,
	std::uniform_real_distribution<T>,
	std::conditional_t<
		std::is_integral_v<T>,
		std::uniform_int_distribution<T>,
		void>
>;

template <class T>
T GetRandom(T start, T end)
{
	static thread_local std::mt19937_64 mt(std::random_device{}());
	uniform_distribution<T> dist(start, end);

	return dist(mt);
}


void GameLevel::Load(std::string file, float levelWidth, float levelHeight)
{
	unsigned int tileCode;

	std::string line;
	std::ifstream fstream(file.c_str());
	if (fstream)
	{
		std::vector<std::vector<unsigned int>> bricks_matrix;
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode)
				row.push_back(tileCode);

			bricks_matrix.push_back(row);
		}
		if (!bricks_matrix.empty())
		{
			init(bricks_matrix, levelWidth, levelHeight);
		}
	}
	else
	{
		load_random_level(levelWidth, levelHeight);
	}
}

void GameLevel::load_random_level(float levelWidth, float levelHeight)
{
	std::vector<std::vector<unsigned int>> bricks_matrix;

	const int column_numbers = GetRandom(10, 25);
	int row_numbers = GetRandom(3, 10);

	while (row_numbers != 0)
	{
		std::vector<unsigned int> row(column_numbers);
		std::generate(row.begin(), row.end(), []() { return GetRandom(0, COLORS_NUMBER); });
		bricks_matrix.emplace_back(row);
		row_numbers--;
	}

	if (!bricks_matrix.empty())
	{
		init(bricks_matrix, levelWidth, levelHeight);
	}
}

void GameLevel::init(std::vector<std::vector<unsigned int>> bricks_matrix, float levelWidth, float levelHeight)
{
	const unsigned int height = bricks_matrix.size();
	const unsigned int width = bricks_matrix[0].size();

	const float unit_width = levelWidth / width;
	const float unit_height = levelHeight / height;

	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			glm::vec2 position(unit_width * x, unit_height * y);
			glm::vec2 size(unit_width, unit_height);

			if (bricks_matrix[y][x] == 1)
			{
				auto& brick(manager.add_entity());
				brick.add_component<TransformComponent>(position, size);
				brick.add_component<StaticComponent>("block_solid", position, size);

				auto& brick_collider = brick.add_component<ColliderComponent>(
					GameEnums::Entities::DamagableType::IMMORTAL);
				brick_collider.pCollider = std::make_shared<BoxCollider>(BoxCollider(size, true));

				brick.add_group(GameEnums::Entities::GroupType::BRICK);
			}
			else if (bricks_matrix[y][x] > 1)
			{
				int brick_lives = bricks_matrix[y][x] - 1;
				int color_index = brick_lives % COLORS_NUMBER;

				auto& brick(manager.add_entity());
				brick.add_component<TransformComponent>(position, size);
				brick.add_component<StaticComponent>("block", position, size, color_index);

				auto& brick_collider = brick.add_component<ColliderComponent>(
					GameEnums::Entities::DamagableType::DESTROYABLE, brick_lives);
				brick_collider.pCollider = std::make_shared<BoxCollider>(BoxCollider(size, true));
				brick.add_group(GameEnums::Entities::GroupType::BRICK);
			}
		}
	}
}
