#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_


#include "game_state.h"
#include "play_state.h"
#include "highscore_state.h"

class MenuState final : public GameState
{
public:
	~MenuState() override;

	static MenuState* GetInstance(StateManager* manager);

	void draw() override;
	void update(float dt) override;
	void on_key(float dt, int key) override;

	void on_char(unsigned int key) override
	{
	}

	void init();

protected:
	MenuState(StateManager* manager);

private:
	void selection_up();
	void selection_down();
	void selection_chosen() const;

	int current_selection;
};

#endif
