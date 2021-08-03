#pragma once
#include <SFML/Graphics.hpp>
#include "Visualiser.hpp"

class Renderer {
private:
	sf::RenderWindow window;
	bool running;
	bool paused;
	sf::Clock clock;
	sf::Font font;

	Visualiser visual;

	void initWindow();
	void initButtons();
public:
	Renderer();
	~Renderer();

	bool isRunning();

	void update();
	void pollEvents();
	void render();

	sf::Vector2f getMousePosition();
	sf::Vector2f getWinSize();
};
