#ifndef _HIGHSCORE_STATE_H_
#define _HIGHSCORE_STATE_H_

#include "game_state.h"
#include <string>
#include <vector>

class HighScoreState final : public GameState
{
public:
	~HighScoreState() override;

	static HighScoreState* GetInstance(StateManager* manager);

	void draw() override;
	void update(float dt) override;
	void on_key(float dt, int key) override;
	void on_char(unsigned int key) override;

	void on_key_down(int wKey);
	void enter_state() override;

	void set_new_highscore(int new_highscore);

	void init();


protected:
	HighScoreState(StateManager* manager);

private:
	void save_scores();
	void add_new_score(const std::string& name, int score);

	struct HighScoreData
	{
		std::string m_player_name;
		int m_score{};

		bool operator<(const HighScoreData& other) const
		{
			if (this->m_score > other.m_score)
				return true;
			return false;
		}
	};

	int m_new_highscore;
	bool m_is_enter_name;
	char m_current_name[26];
	int m_char_name_index;

	using HighScoreTable = std::vector<HighScoreData>;
	HighScoreTable m_vector_highscores;
};

#endif
