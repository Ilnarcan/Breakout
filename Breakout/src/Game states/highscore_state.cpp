#include <fstream>
#include <sstream>
#include <algorithm>
#include "highscore_state.h"

#include "game.h"
#include "menu_state.h"
#include "components.h"
#include "render_system.h"

const glm::vec3 COLOR_SELECTED(242.0f / 255.0f, 155.0f / 255.0f, 77.0f / 255.0f);

const int HIGHSCORE_SIZE = 10;

HighScoreState::HighScoreState(StateManager* manager)
	: GameState(manager), m_new_highscore(0), m_is_enter_name(false), m_current_name(""),
	  m_char_name_index(0)
{
	init();
}

HighScoreState::~HighScoreState()
{
}

void HighScoreState::init()
{
	manager.reset();

	game_state = GameEnums::GameState::HIGHSCORE;

	auto& background(manager.add_entity());
	background.add_component<StaticComponent>("menu-background", glm::vec2(0.0f, 0.0f),
	                                         glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	background.add_group(GameEnums::Entities::IMG);

	auto& panel(manager.add_entity());
	panel.add_component<StaticComponent>("menu-panel", glm::vec2(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT / 2 - 250.0f),
	                                    glm::vec2(600.0f, 500.0f));
	panel.add_group(GameEnums::Entities::IMG);
}

void HighScoreState::set_new_highscore(int new_highscore)
{
	m_new_highscore = new_highscore;
}

HighScoreState* HighScoreState::GetInstance(StateManager* manager)
{
	static HighScoreState instance(manager);
	return &instance;
}

void HighScoreState::update(float dt)
{
}

void HighScoreState::draw()
{
	RenderSystem::draw();

	float y_pos = 135.0f;
	const float x_pos_name = SCREEN_WIDTH / 2 - 250.0f;
	const float x_pos_score = SCREEN_WIDTH / 2 + 230.0f;

	int place = 1;

	for (const auto& x : m_vector_highscores)
	{
		Game::text_renderer->render_text(std::to_string(place++) + ". " + x.m_player_name, x_pos_name, y_pos, 0.5f,
		                                 false);
		Game::text_renderer->render_text(std::to_string(x.m_score), x_pos_score, y_pos, 0.5f, false);

		y_pos += 35;
	}

	if (m_is_enter_name)
	{
		Game::sprite_renderer->draw_sprite(Game::assets->get_texture("menu-panel"),
		                                   glm::vec2(SCREEN_WIDTH / 2 - 300.0f, SCREEN_HEIGHT / 2 - 125.0f),
		                                   glm::vec2(600.0f, 300.0f));
		Game::text_renderer->render_text("Enter your name", SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f - 90.0f, 0.7f, true);
		Game::text_renderer->render_text("______________________", SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f, 0.7f, true);
		Game::text_renderer->render_text(m_current_name, SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f, 0.6f, true,
		                                 COLOR_SELECTED);
	}
}

void HighScoreState::on_key(float dt, int key)
{
	if (m_is_enter_name)
	{
		switch (key)
		{
			// In case of a return, the new score should be added.
		case GLFW_KEY_ENTER:
			{
				if (static_cast<std::string>(m_current_name).empty())
					break;

				add_new_score(m_current_name, m_new_highscore);
				m_new_highscore = 0;
				m_is_enter_name = false;
				m_char_name_index = 0;
				m_current_name[0] = '\0';
				break;
			}
		case GLFW_KEY_BACKSPACE:
			{
				// Remove one character
				if (m_char_name_index > 0)
				{
					m_char_name_index--;
					m_current_name[m_char_name_index] = '\0';
				}
				break;
			}
		default: ;
		}
	}
	else
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_ENTER:
			{
				MenuState* menu_state = MenuState::GetInstance(stateManager);
				menu_state->init();
				change_state(menu_state);

				break;
			}
		default: ;
		}
	}
}

void HighScoreState::on_key_down(int wKey)
{
}

void HighScoreState::on_char(unsigned int key)
{
	if (m_is_enter_name && (m_char_name_index < 25))
	{
		// Filter the characters for only alphabetical characters.
		if ((key >= 64 && key <= 91) ||
			(key >= 97 && key <= 122))
		{
			m_current_name[m_char_name_index] = key;
			m_char_name_index++;
			m_current_name[m_char_name_index] = '\0';
		}
	}
}

void HighScoreState::enter_state()
{
	// Clear the highscore table
	m_vector_highscores.clear();
	std::ifstream input_file("HighScores.txt");
	if (input_file.fail())
	{
		if (m_new_highscore)
			m_is_enter_name = true;
		return;
	}

	std::string line;
	HighScoreData new_score;
	while (std::getline(input_file, line))
	{
		std::istringstream iss(line);
		iss >> new_score.m_player_name >> new_score.m_score;
		if (new_score.m_score > 0)
		{
			if (new_score.m_player_name.empty())
			{
				new_score.m_player_name = "Nameless hero";
			}
		}

		m_vector_highscores.push_back(new_score);
	}

	sort(m_vector_highscores.begin(), m_vector_highscores.end());

	// Check if we have a new highscore that should be 
	// added in the table. If yes, m_is_enter_name is set to true.
	int last_score = 0;
	if (!m_vector_highscores.empty())
		last_score = m_vector_highscores[m_vector_highscores.size() - 1].m_score;
	if (m_new_highscore && (m_new_highscore > last_score || m_vector_highscores.size() < HIGHSCORE_SIZE))
		m_is_enter_name = true;
}

void HighScoreState::save_scores()
{
	// Create the file
	std::ofstream output_file("HighScores.txt");
	if (output_file.fail())
		return;

	for (const auto& var : m_vector_highscores)
	{
		output_file << var.m_player_name << " " << var.m_score << std::endl;
	}
}

void HighScoreState::add_new_score(const std::string& name, int score)
{
	// Create a new highscore and push it into the table 
	HighScoreData new_data;
	new_data.m_player_name = name;
	new_data.m_score = score;
	m_vector_highscores.push_back(new_data);

	sort(m_vector_highscores.begin(), m_vector_highscores.end());

	// If too much elements, remove the last one.
	while (m_vector_highscores.size() > HIGHSCORE_SIZE)
		m_vector_highscores.pop_back();

	save_scores();
}
