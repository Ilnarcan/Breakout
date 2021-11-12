#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include "game.h"
#include "components.h"

extern Manager manager;

class RenderSystem
{
public:
	static void draw();

	static void draw(const std::vector<Entity*>& entities);

};

#endif
