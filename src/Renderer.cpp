#include "Renderer.hpp"
#include "GUI.hpp"

void Renderer::initWindow()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	window.create(sf::VideoMode(1200, 700), "Algorithm Visualiser", sf::Style::Default, settings);
	window.setFramerateLimit(120);
}

void Renderer::initButtons()
{
	font.loadFromFile("data/consola.ttf");

	sf::RectangleShape rect, highlight;
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(sf::Color(160, 32, 240));
	rect.setOutlineThickness(3);

	highlight.setFillColor(sf::Color(160, 32, 240, 126));
	highlight.setOutlineColor(sf::Color(124, 163, 251));
	highlight.setOutlineThickness(5);

	sf::Text text;
	text.setCharacterSize(22);
	text.setFillColor(sf::Color::White);

	float gap = 10;
	sf::Vector2f size(100, 40);
	int index = 0;

	gui::Page::Group["Page"] = gui::Page::create({ this->getWinSize().x * 1.5f , size.y });
	gui::Page::Group["Page"]->setPosition(0, getWinSize().y - 20 - size.y);
	gui::Page::Group["Page"]->setMousePositionFunction([this]() {return this->getMousePosition(); });

	auto constructButton = [this, &rect, &highlight, &text, &gap, &size, &index](const std::string& name) {
		gui::Page::Group["Page"]->Buttons[name] = gui::Button::create(size);
		gui::Page::Group["Page"]->Buttons[name]->setPosition(sf::Vector2f(10 + index * (gap + size.x), 0));
		gui::Page::Group["Page"]->Buttons[name]->setFillColor(rect.getFillColor());
		gui::Page::Group["Page"]->Buttons[name]->setOutlineColor(rect.getOutlineColor());
		gui::Page::Group["Page"]->Buttons[name]->setOutlineThickness(rect.getOutlineThickness());
		gui::Page::Group["Page"]->Buttons[name]->setHighlightFillColor(highlight.getFillColor());
		gui::Page::Group["Page"]->Buttons[name]->setHighlightOutlineColor(highlight.getOutlineColor());
		gui::Page::Group["Page"]->Buttons[name]->setHighlightOutlineThickness(highlight.getOutlineThickness());
		gui::Page::Group["Page"]->Buttons[name]->setCharacterSize(text.getCharacterSize());
		gui::Page::Group["Page"]->Buttons[name]->setTextFillColor(text.getFillColor());
		gui::Page::Group["Page"]->Buttons[name]->setFont(font);
		gui::Page::Group["Page"]->Buttons[name]->setString(name);
		index++;
	};

	constructButton("Reset");
	gui::Page::Group["Page"]->Buttons["Reset"]->setAction([this]() {visual.create(this->getWinSize()); });
	gui::Page::Group["Page"]->Buttons["Reset"]->setOutlineColor(sf::Color::Red);
	gui::Page::Group["Page"]->Buttons["Reset"]->setHighlightFillColor(sf::Color(255,0,0,126));
	gui::Page::Group["Page"]->Buttons["Reset"]->setHighlightOutlineColor(sf::Color(255,165,0));

	constructButton("Load");
	gui::Page::Group["Page"]->Buttons["Load"]->setAction([this]() {visual.loadAgain(); });
	gui::Page::Group["Page"]->Buttons["Load"]->setOutlineColor(sf::Color::Red);
	gui::Page::Group["Page"]->Buttons["Load"]->setHighlightFillColor(sf::Color(255, 0, 0, 126));
	gui::Page::Group["Page"]->Buttons["Load"]->setHighlightOutlineColor(sf::Color(255, 165, 0));

	constructButton("Bubble");
	gui::Page::Group["Page"]->Buttons["Bubble"]->setAction([this]() {visual.setAlgorithm(Visualiser::BUBBLE); });

	constructButton("Select");
	gui::Page::Group["Page"]->Buttons["Select"]->setAction([this]() {visual.setAlgorithm(Visualiser::SELECTION); });

	constructButton("Insert");
	gui::Page::Group["Page"]->Buttons["Insert"]->setAction([this]() {visual.setAlgorithm(Visualiser::INSERTION); });

	constructButton("Quick ll");
	gui::Page::Group["Page"]->Buttons["Quick ll"]->setAction([this]() {visual.setAlgorithm(Visualiser::QUICK_LL); });

	constructButton("Quick lr");
	gui::Page::Group["Page"]->Buttons["Quick lr"]->setAction([this]() {visual.setAlgorithm(Visualiser::QUICK_LR); });

	constructButton("Merge");
	gui::Page::Group["Page"]->Buttons["Merge"]->setAction([this]() {visual.setAlgorithm(Visualiser::MERGE); });

	constructButton("Merge 2");
	gui::Page::Group["Page"]->Buttons["Merge 2"]->setAction([this]() {visual.setAlgorithm(Visualiser::MERGE_2); });

	gui::Page::Group["Page"]->Sliders["Speed"] = gui::Slider::create(size);
	gui::Page::Group["Page"]->Sliders["Speed"]->setPosition(sf::Vector2f(10 + index * (gap + size.x), 0));
	gui::Page::Group["Page"]->Sliders["Speed"]->setFillColor(sf::Color::Transparent);
	gui::Page::Group["Page"]->Sliders["Speed"]->setOutlineColor(sf::Color::Blue);
	gui::Page::Group["Page"]->Sliders["Speed"]->setOutlineThickness(3);
	gui::Page::Group["Page"]->Sliders["Speed"]->setTextFillColor(sf::Color::White);
	gui::Page::Group["Page"]->Sliders["Speed"]->setFont(font);
	gui::Page::Group["Page"]->Sliders["Speed"]->setCharacterSize(22);
	gui::Page::Group["Page"]->Sliders["Speed"]->setString("Speed");
	gui::Page::Group["Page"]->Sliders["Speed"]->action = [this]() {
		gui::Page::Group["Page"]->Sliders["Speed"]->moveSliderBar(gui::Page::Group["Page"]->getMousePositionInPage());
		visual.setSpeed(gui::Page::Group["Page"]->Sliders["Speed"]->getMappedOffset() * visual.getSpeedLimit());
	};
	index++;

	gui::Page::Group["Page"]->Sliders["Width"] = gui::Slider::create(size);
	gui::Page::Group["Page"]->Sliders["Width"]->setPosition(sf::Vector2f(10 + index * (gap + size.x), 0));
	gui::Page::Group["Page"]->Sliders["Width"]->setFillColor(sf::Color::Transparent);
	gui::Page::Group["Page"]->Sliders["Width"]->setOutlineColor(sf::Color::Blue);
	gui::Page::Group["Page"]->Sliders["Width"]->setOutlineThickness(3);
	gui::Page::Group["Page"]->Sliders["Width"]->setTextFillColor(sf::Color::White);
	gui::Page::Group["Page"]->Sliders["Width"]->setFont(font);
	gui::Page::Group["Page"]->Sliders["Width"]->setCharacterSize(22);
	gui::Page::Group["Page"]->Sliders["Width"]->setString("Width");
	gui::Page::Group["Page"]->Sliders["Width"]->action = [this]() {
		gui::Page::Group["Page"]->Sliders["Width"]->moveSliderBar(gui::Page::Group["Page"]->getMousePositionInPage());
		visual.setWidth(gui::Page::Group["Page"]->Sliders["Width"]->getMappedOffset() * visual.getWidthLimit());
	};
	index++;

	gui::Page::Group["Page"]->Textboxes["time"] = gui::Textbox::create({ 80, 40 });
	gui::Page::Group["Page"]->Textboxes["time"]->setPosition(sf::Vector2f(10 + index * (gap + size.x), 0));
	gui::Page::Group["Page"]->Textboxes["time"]->setFont(font);
	gui::Page::Group["Page"]->Textboxes["time"]->setFillColor(sf::Color::Transparent);
	gui::Page::Group["Page"]->Textboxes["time"]->setOutlineColor(sf::Color::Transparent);
	gui::Page::Group["Page"]->Textboxes["time"]->setCharacterSize(22);
	gui::Page::Group["Page"]->Textboxes["time"]->setTextFillColor(sf::Color::White);
	gui::Page::Group["Page"]->Textboxes["time"]->setString("");

	gui::Scroll::Group["Scroll"] = gui::Scroll::create(this->getWinSize().x);
	gui::Scroll::Group["Scroll"]->setHorizontal();
	gui::Scroll::Group["Scroll"]->setAt(0, this->getWinSize().y);
	gui::Scroll::Group["Scroll"]->attach(&gui::Page::Group["Page"]);
	gui::Scroll::Group["Scroll"]->getMousePosition = [this]() {
		return this->getMousePosition();
	};
}

Renderer::Renderer()
{
	this->initWindow();
	this->initButtons();

	running = true;
	paused = false;

	visual.create(this->getWinSize());
}

Renderer::~Renderer()
{
	gui::deleteAllGuiGroups();
}

bool Renderer::isRunning()
{
	return this->running;
}

void Renderer::update()
{
	float dt = clock.getElapsedTime().asSeconds();
	clock.restart();

	this->pollEvents();

	if (!this->paused) {
		visual.update(dt);
	}
}

void Renderer::pollEvents()
{
	sf::Event event;
	sf::Vector2f mousePos = this->getMousePosition();

	if(!this->paused)gui::updateMousePointer(this->getMousePosition());

	while (this->window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			this->running = false;

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)this->paused = !this->paused;

		if (!this->paused) {
			if (event.type == sf::Event::MouseMoved) {
				
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				gui::registerClick(event.mouseButton.button);
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				gui::unregisterClick(event.mouseButton.button, this->getMousePosition());
			}
			else if (event.type == sf::Event::KeyPressed) {
				
			}
		}
	}
}

void Renderer::render()
{
	this->window.clear();

	gui::render(this->window);
	visual.draw(this->window);

	this->window.display();
}

sf::Vector2f Renderer::getMousePosition()
{
	return window.mapPixelToCoords(sf::Mouse::getPosition(this->window));
}

sf::Vector2f Renderer::getWinSize()
{
	return sf::Vector2f(this->window.getSize());
}