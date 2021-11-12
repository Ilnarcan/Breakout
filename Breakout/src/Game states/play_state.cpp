
#include <sstream>
#include "play_state.h"
#include "highscore_state.h"
#include "components.h"
#include "box_collider.h"
#include "game_level.h"
#include "sphere_collider.h"
#include "collision_system.h"
#include "gameplay_system.h"
#include "movement_system.h"
#include "render_system.h"


const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float BALL_RADIUS = 12.5f;
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const glm::vec2 PLAYER_VELOCITY(200.0f, 0.0f);


PlayState::PlayState(StateManager* state_manager)
	: GameState(state_manager), m_score(0), m_level(1)
{
	Game::assets->add_texture("player", "resource/paddle.png");
	Game::assets->add_texture("face", "resource/awesomeface.png");
	Game::assets->add_texture("background", "resource/background.jpg", false);
	Game::assets->add_texture("block", "resource/block.png", false);
	Game::assets->add_texture("block_solid", "resource/block_solid.png", false);

	init();
}

PlayState::~PlayState()
{
}

PlayState* PlayState::GetInstance(StateManager* manager)
{
	static PlayState instance(manager);
	return &instance;
}

void PlayState::update(float dt)
{
	if (game_state != GameEnums::GameState::GAME_ACTIVE)
	{
		return;
	}

	manager.refresh();

	MovementSystem::update(dt);
	CollisionSystem::update(dt, m_score);
	GameplaySystem::update(game_state);

	if (game_state == GameEnums::GameState::GAME_OVER)
	{
		return;
	}

	check_game_state();
}

void PlayState::draw()
{
	RenderSystem::draw();

	if (game_state == GameEnums::GameState::GAME_PAUSE)
	{
		Game::text_renderer->render_text("Press SPACE to start", SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f - 50.0f, 0.7f,
		                                 true);
	}

	std::stringstream ss;
	ss << m_score;
	Game::text_renderer->render_text("Score:" + ss.str(), 5.0f, 5.0f, 1.0f);

	if (game_state == GameEnums::GameState::GAME_OVER)
	{
		Game::text_renderer->render_text("GAME OVER", SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f - 90.0f, 0.9f, true);
		Game::text_renderer->render_text("Press ENTER to continue", SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f, 0.7f, true);
	}
}

void PlayState::on_key(float dt, int key)
{
	switch (key)
	{
	case(GLFW_KEY_SPACE):
		{
			if (game_state == GameEnums::GameState::GAME_PAUSE)
			{
				game_state = GameEnums::GameState::GAME_ACTIVE;
			}

			break;
		}
	case(GLFW_KEY_A):
		{
			if (game_state == GameEnums::GameState::GAME_ACTIVE)
			{
				MovementSystem::move(-PLAYER_VELOCITY, dt, key);
			}

			break;
		}
	case(GLFW_KEY_D):
		{
			if (game_state == GameEnums::GameState::GAME_ACTIVE)
			{
				MovementSystem::move(PLAYER_VELOCITY, dt, key);
			}

			break;
		}
	case(GLFW_KEY_ENTER):
		{
			if (game_state == GameEnums::GameState::GAME_OVER)
			{
				HighScoreState* highscore_state = HighScoreState::GetInstance(stateManager);
				highscore_state->init();
				highscore_state->set_new_highscore(m_score);
				change_state(highscore_state);
			}
			break;
		}
	default: ;
	}
}

void PlayState::init()
{
	manager.reset();

	if (game_state != GameEnums::GameState::GAME_WIN)
	{
		m_score = 0;
		m_level = 1;
		load_level();
	}

	game_state = GameEnums::GameState::GAME_PAUSE;

	auto& background(manager.add_entity());
	background.add_component<StaticComponent>("background", glm::vec2(0.0f, 0.0f),
	                                         glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	background.add_group(GameEnums::Entities::IMG);

	auto playerPos = glm::vec2(SCREEN_WIDTH / 2.0f - PLAYER_SIZE.x / 2.0f, SCREEN_HEIGHT - PLAYER_SIZE.y);
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);

	auto& player(manager.add_entity());
	player.add_component<TransformComponent>(playerPos, PLAYER_SIZE);
	player.add_component<SpriteComponent>("player");
	auto& player_collider = player.add_component<ColliderComponent>(GameEnums::Entities::DamagableType::IMMORTAL);
	player_collider.pCollider = std::make_shared<BoxCollider>(BoxCollider(PLAYER_SIZE, true));
	player.add_group(GameEnums::Entities::PLAYER);

	auto& ball(manager.add_entity());
	ball.add_component<TransformComponent>(ballPos, glm::vec2{ BALL_RADIUS * 2.0f,BALL_RADIUS * 2.0f }, INITIAL_BALL_VELOCITY);
	ball.add_component<BallComponent>("face");
	auto& ball_collider = ball.add_component<ColliderComponent>(GameEnums::Entities::DamagableType::IMMORTAL);
	ball_collider.pCollider = std::make_shared<SphereCollider>(SphereCollider(BALL_RADIUS, true));
	ball.add_group(GameEnums::Entities::BALL);
}

void PlayState::check_game_state()
{
	if (game_state == GameEnums::GameState::GAME_WIN)
	{
		m_level++;

		init();
		load_level();
	}
}

void PlayState::load_level() const

{
	GameLevel::Load("levels/" + std::to_string(m_level) + ".lvl", SCREEN_WIDTH, SCREEN_HEIGHT / 2);
}
