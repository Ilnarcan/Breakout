#ifndef _GAME_ENUMS_H
#define _GAME_ENUMS_H

namespace GameEnums
{
	namespace Entities
	{
		enum GroupType : std::size_t
		{
			BRICK,
			PLAYER,
			COLLIDER,
			BALL,
			LABEL,
			IMG,
			GAME_IMG,
			GAME_LABEL
		};

		enum class FigureType
		{
			NONE,
			CIRCLE,
			RECTANGLE
		};

		enum class DamagableType
		{
			NONE,
			DESTROYABLE,
			IMMORTAL
		};
	}

	enum class GameState
	{
		NONE,
		GAME_PAUSE,
		GAME_ACTIVE,
		GAME_WIN,
		GAME_OVER,
		MENU,
		HIGHSCORE
	};
}

#endif
