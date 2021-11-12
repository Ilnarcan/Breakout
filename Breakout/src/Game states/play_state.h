#ifndef _PLAY_STATE_H_
#define _PLAY_STATE_H_

#include <vector>

#include "ECS.h"
#include "game_state.h"
#include "game_enums.h"
#include "game_level.h"

class PlayState final : public GameState
{
public:
	~PlayState() override;

	static PlayState* GetInstance(StateManager* manager);

	void draw() override;
	void update(float dt) override;
	void on_key(float dt, int key) override;

	void on_char(unsigned int key) override
	{
	}

	void init();

protected:
	PlayState(StateManager* manager);

private:
	void check_game_state();
	void load_level() const;

	int m_score;
	int m_level;
};


#endif
