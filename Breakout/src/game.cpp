#include "game.h"
#include "window_manager.h"
#include "GLFW/glfw3.h"

SpriteRenderer* Game::sprite_renderer;
TextRenderer* Game::text_renderer;

Manager manager;

AssetManager* Game::assets = new AssetManager(&manager);

Game::Game()
{
}

Game::Game(float width, float height, const char* title)
	: m_width(width), m_height(height), m_title(title)
{
}

Game::~Game()
{
	delete sprite_renderer;
	delete text_renderer;
}

void Game::init()
{
	WindowManager* window = WindowManager::GetInstance();

	WindowManager::init(m_width, m_height, m_title);

	// Loading shaders
	assets->load_shader("shaders/shader.vs", "shaders/shader.fsh", nullptr, "sprite");
	assets->load_shader("shaders/text.vs", "shaders/text.fsh", nullptr, "text");

	// Configuring shaders
	assets->get_shader("sprite").use();
	assets->get_shader("sprite").set_integer("image", 0);
	assets->get_shader("sprite").set_matrix4("projection", glm::ortho(0.0f, m_width, m_height, 0.0f, -1.0f, 1.0f));
	assets->get_shader("text").set_matrix4("projection", glm::ortho(0.0f, m_width, m_height, 0.0f), true);
	assets->get_shader("text").set_integer("text", 0);

	sprite_renderer = new SpriteRenderer(assets->get_shader("sprite"));

	text_renderer = new TextRenderer(assets->get_shader("text"));
	text_renderer->load("fonts/comicbd.ttf", 50);

	// Start from Menu
	window->change_state();
}

void Game::start()
{
	WindowManager* window = WindowManager::GetInstance();

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window->m_window))
	{
		// Вычисление дельты времени
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// Обрабатываем пользовательский ввод с клавиатуры
		window->on_key(deltaTime);

		// Обновляем состояние игры
		window->update(deltaTime);

		// Рендер
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		window->draw();

		glfwSwapBuffers(window->m_window);
	}

	// Освобождение всех ресурсов, загруженных с использованием менеджера ресурсов
	assets->clear();
}
