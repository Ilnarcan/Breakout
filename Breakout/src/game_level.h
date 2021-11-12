#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

#include <string>
#include <vector>

class GameLevel
{
public:
	GameLevel()
	{
	}

	~GameLevel()
	{
	}

	static void Load(std::string file, float levelWidth, float levelHeight);

private:
	static void init(std::vector<std::vector<unsigned int>> tileData, float levelWidth, float levelHeight);
	static void load_random_level(float levelWidth, float levelHeight);
};

#endif
