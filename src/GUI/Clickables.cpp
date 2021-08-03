#include "Clickables.hpp"

namespace gui {

	Clickable* Clickable::mouseHoveringOn = nullptr;
	sf::Mouse::Button Clickable::pressedMouseButton;
	Clickable* Clickable::clicked = nullptr;

	Clickable::Clickable()
		:state(ORIGINAL)
	{
		
	}
	Clickable::~Clickable()
	{
	}
	void Clickable::setAction(std::function<void()> func)
	{
		action = func;
	}
}