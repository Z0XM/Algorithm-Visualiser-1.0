#include "Page.hpp"
#include "Scroll.hpp"

std::unordered_map<std::string, gui::Page*> gui::Page::Group;

gui::Page::Page(const sf::Vector2f& size)
{
	this->setSize(size);
	setFillColor(sf::Color::Transparent);
	setOutlineColor(sf::Color::Transparent);

	scroll = nullptr;
	active = true;
}

template <typename T>
void deleteGroup(std::unordered_map<std::string, T*>& map)
{
	for (auto it = map.begin(); it != map.end(); it++) {
		delete it->second;
	}
	map.clear();
}

gui::Page::~Page()
{
	deleteGroup(Textboxes);
	deleteGroup(Buttons);
	deleteGroup(Sliders);
	if(scroll != nullptr)scroll->dettach();
}

void gui::Page::setSize(const sf::Vector2f& size)
{
	box.setSize(size);
}

const sf::Vector2f& gui::Page::getSize() const
{
	return box.getSize();
}

void gui::Page::setFillColor(const sf::Color& color)
{
	box.setFillColor(color);
}

void gui::Page::setOutlineColor(const sf::Color& color)
{
	box.setOutlineColor(color);
}

void gui::Page::setOutlineThickness(float thickness)
{
	box.setOutlineThickness(thickness);
}

const sf::Color& gui::Page::getFillColor() const
{
	return box.getFillColor();
}

const sf::Color& gui::Page::getOutlineColor() const
{
	return 	box.getOutlineColor();
}

float gui::Page::getOutlineThickness() const
{
	return box.getOutlineThickness();
}

void gui::Page::setPosition(float x, float y)
{
	box.setPosition(x, y);
}

void gui::Page::setPosition(const sf::Vector2f& position)
{
	box.setPosition(position);
}

const sf::Vector2f& gui::Page::getPosition() const
{
	return box.getPosition();
}

void gui::Page::move(float offsetX, float offsetY)
{
	box.move(offsetX, offsetY);
}

void gui::Page::move(const sf::Vector2f& offset)
{
	box.move(offset);
}

gui::Clickable* gui::Page::isHit(const sf::Vector2f& mousePos)
{
	if (!active)return nullptr;

	Clickable* clickable;
	for (auto it = Buttons.begin(); it != Buttons.end(); it++) {
		clickable = it->second->isHit(mousePos - getPosition());
		if (clickable != nullptr)return clickable;
	}
	for (auto it = Sliders.begin(); it != Sliders.end(); it++) {
		clickable = it->second->isHit(mousePos - getPosition());
		if (clickable != nullptr)return clickable;
	}
	return nullptr;
}


void gui::Page::setScroll(gui::Scroll* scroll)
{
	this->scroll = scroll;
}

void gui::Page::setActive()
{
	active = true;
}

void gui::Page::setInactive()
{
	active = false;
}

void gui::Page::setMousePositionFunction(std::function<sf::Vector2f()> func)
{
	getMousePositionInPage = [this, func] {return func() - sf::Vector2f(getPosition().x, 0);};
}

template <typename T>
void drawGroup(gui::Page* page, std::unordered_map<std::string, T*>& group, sf::RenderTarget& target)
{
	for (auto it = group.begin(); it != group.end(); it++) {
		it->second->move(page->getPosition());
		it->second->draw(target);
		it->second->move(-page->getPosition());
	}
}

void gui::Page::draw(sf::RenderTarget& target)
{
	if (active) {
		target.draw(box);
		drawGroup(this, Buttons, target);
		drawGroup(this, Sliders, target);
		drawGroup(this, Textboxes, target);
	}
}

gui::Page* gui::Page::create(const sf::Vector2f& size)
{
	return new Page(size);
}

gui::Page* gui::Page::createCopy(const Page& page)
{
	return new Page(page);
}
