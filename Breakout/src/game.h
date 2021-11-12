#ifndef _GAME_H
#define _GAME_H

#include "sprite_renderer.h"
#include "text_renderer.h"
#include "asset_manager.h"
#include "ECS.h"

class SpriteRenderer;
class TextRenderer;
class AssetManager;

extern const float SCREEN_WIDTH;
extern const float SCREEN_HEIGHT;

class Game
{
public:
	Game();
	Game(float width, float height, const char* title);

	~Game();

	void init();
	void start();

	static SpriteRenderer* sprite_renderer;
	static TextRenderer* text_renderer;
	static AssetManager* assets;

private:
	float m_width, m_height;
	const char* m_title;
};

#endif
