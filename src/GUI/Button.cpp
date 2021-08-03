#include "Button.hpp"
#pragma once

std::unordered_map<std::string, gui::Button*> gui::Button::Group;

gui::Button::Button(const sf::Vector2f& size)
	:Textbox(size)
{
	action = nullptr;
	actionEvent = UNREGISTER;
}

gui::Button::~Button()
{
}

void gui::Button::setHighlightFillColor(const sf::Color& color)
{
	highlight.fillColor = color;
	if (state == HIGHLIGHT)box.setFillColor(color);
}

void gui::Button::setHighlightOutlineColor(const sf::Color& color)
{
	highlight.outlineColor = color;
	if (state == HIGHLIGHT)box.setOutlineColor(color);
}

void gui::Button::setHighlightOutlineThickness(float thickness)
{
	highlight.outlineThickness = thickness;
	if (state == HIGHLIGHT)box.setOutlineThickness(thickness);
}

void gui::Button::setFillColor(const sf::Color& color)
{
	original.fillColor = color;
	if (state == ORIGINAL)box.setFillColor(color);
}

void gui::Button::setOutlineColor(const sf::Color& color)
{
	original.outlineColor = color;
	if (state == ORIGINAL)box.setOutlineColor(color);
}

void gui::Button::setOutlineThickness(float thickness)
{
	original.outlineThickness = thickness;
	if (state == ORIGINAL)box.setOutlineThickness(thickness);
}

bool gui::Button::contains(const sf::Vector2f& mousePos) const
{
	return box.getGlobalBounds().contains(mousePos);
}

gui::Clickable* gui::Button::isHit(const sf::Vector2f& mousePos)
{
	return this->contains(mousePos) ? this : nullptr;
}

void gui::Button::activateHighlight()
{
	this->state = HIGHLIGHT;
	box.setFillColor(highlight.fillColor);
	box.setOutlineColor(highlight.outlineColor);
	box.setOutlineThickness(highlight.outlineThickness);
}
void gui::Button::deactivateHighlight()
{
	this->state = ORIGINAL;
	box.setFillColor(original.fillColor);
	box.setOutlineColor(original.outlineColor);
	box.setOutlineThickness(original.outlineThickness);
}

void gui::Button::draw(sf::RenderTarget& target)
{
	target.draw(box);
	target.draw(text);
}

gui::Button* gui::Button::create(const sf::Vector2f& size)
{
	return new Button(size);
}

gui::Button* gui::Button::createCopy(const gui::Button& button)
{
	return new Button(button);
}
