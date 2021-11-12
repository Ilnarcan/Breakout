#include "menu_state.h"

#include "window_manager.h"
#include "game.h"
#include "components.h"
#include "render_system.h"

const glm::vec3 COLOR_SELECTED(242.0f / 255.0f, 155.0f / 255.0f, 77.0f / 255.0f);

MenuState::MenuState(StateManager* state_manager)
	: GameState(state_manager), current_selection(0)
{
	Game::assets->add_texture("menu-background", "resource/menu-high-background.png", false);
	Game::assets->add_texture("menu-panel", "resource/menu-panel.jpg", false);
	Game::assets->add_texture("menu-img", "resource/menu-img.png");
	Game::assets->add_texture("menu-selected", "resource/menu-selected.png");

	init();
}

MenuState::~MenuState()
{
}

MenuState* MenuState::GetInstance(StateManager* state_manager)
{
	static MenuState instance(state_manager);
	return &instance;
}

void MenuState::init()
{
	manager.reset();

	game_state = GameEnums::GameState::MENU;

	auto& background(manager.add_entity());
	background.add_component<StaticComponent>("menu-background", glm::vec2(0.0f, 0.0f),
	                                         glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	background.add_group(GameEnums::Entities::IMG);

	auto& panel(manager.add_entity());
	panel.add_component<StaticComponent>("menu-panel", glm::vec2(SCREEN_WIDTH / 2 - 200.0f, SCREEN_HEIGHT / 2 - 250.0f),
	                                    glm::vec2(400.0f, 500.0f));
	panel.add_group(GameEnums::Entities::IMG);

	auto& label(manager.add_entity());
	label.add_component<UIComponent>("New game", glm::vec2{ SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f - 100.0f }, COLOR_SELECTED, 1.0f,
	                                true);
	label.add_group(GameEnums::Entities::LABEL);

	auto& label2(manager.add_entity());
	label2.add_component<UIComponent>("High scores", glm::vec2{ SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f }, COLOR_SELECTED, 1.0f, true);
	label2.add_group(GameEnums::Entities::LABEL);

	auto& label3(manager.add_entity());
	label3.add_component<UIComponent>("Exit", glm::vec2{ SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f + 100.0f }, COLOR_SELECTED, 1.0f, true);
	label3.add_group(GameEnums::Entities::LABEL);
}

void MenuState::draw()
{
	RenderSystem::draw();

	// Draw a frame to the selected menu bar
	for (auto i = 0; i < 3; i++)
	{
		if (i == current_selection)
		{
			Game::sprite_renderer->draw_sprite(Game::assets->get_texture("menu-selected"),
			                                   glm::vec2(SCREEN_WIDTH / 2 - 185.0f,
			                                             SCREEN_HEIGHT / 2 - 100.0f - 20.0f + 100.0f * i),
			                                   glm::vec2(370, 80.0f), 0.0f);
		}
	}
}

void MenuState::update(float dt)
{
}

void MenuState::on_key(float dt, int key)
{
	switch (key)
	{
	case GLFW_KEY_DOWN:
		selection_down();
		break;
	case GLFW_KEY_UP:
		selection_up();
		break;
	case GLFW_KEY_ENTER:
		selection_chosen();
		break;
	default: ;
	}
}

void MenuState::selection_up()
{
	current_selection--;
	current_selection = (current_selection + 3) % 3;
}

void MenuState::selection_down()
{
	current_selection++;
	current_selection = current_selection % 3;
}

void MenuState::selection_chosen() const
{
	switch (current_selection)
	{
	case 0:
		{
			PlayState* new_game = PlayState::GetInstance(stateManager);
			new_game->init();
			change_state(new_game);
			break;
		}

	case 1:
		{
			HighScoreState* highscore_state = HighScoreState::GetInstance(stateManager);
			highscore_state->init();
			change_state(highscore_state);
			break;
		}

	case 2:
		{
			WindowManager::terminate();
			break;
		}
	default: ;
	}
}
