#include "Scroll.hpp"
#include <algorithm>

std::unordered_map<std::string, gui::Scroll*> gui::Scroll::Group;

gui::Scroll::Scroll(float height)
{
	setSize({15, height});
	rect.setFillColor(sf::Color(41, 41, 41));
	rect.setOutlineColor(sf::Color::Transparent);
	rect.setHighlightFillColor(sf::Color(60, 60, 60));
	rect.setHighlightOutlineColor(sf::Color::Transparent);
	
	bar.setFillColor(sf::Color(150, 150, 150, 70));
	bar.setOutlineColor(sf::Color::Transparent);
	bar.setHighlightFillColor(sf::Color(200, 200, 200, 70));
	bar.setHighlightOutlineColor(sf::Color::Transparent);

	rect.setText(false);
	bar.setText(false);

	isHorizontal = false;
	
	rect.actionEvent = Clickable::REGISTER;
	bar.actionEvent = Clickable::MOUSEMOVE;

	rect.action = [this]() {
		scrollTo(getMousePosition());
	};
	bar.action = [this]() {
		scrollTo(getMousePosition());
	};
}

gui::Scroll::~Scroll()
{
}

void gui::Scroll::updateBar()
{
	bar.setSize(getSize());
	bar.setPosition(getPosition());

	if (connectedPage != nullptr){
		sf::Vector2f offset = connectedPage->getSize() - rect.getSize();

		if (isHorizontal) {
			bar.setSize(bar.getSize() - sf::Vector2f(std::max(0.f, offset.x), 0));

			float startOffset = std::max(0.f, rect.getPosition().x - connectedPage->getPosition().x);
			bar.setPosition(rect.getPosition() + sf::Vector2f(startOffset, 0));
		}
		else {
			bar.setSize(bar.getSize() - sf::Vector2f(0, std::max(0.f, offset.y)));

			float startOffset = std::max(0.f, rect.getPosition().y - connectedPage->getPosition().y);
			bar.setPosition(rect.getPosition() + sf::Vector2f(0, startOffset));
		}
	}
}

void gui::Scroll::setSize(const sf::Vector2f& size)
{
	rect.setSize(size);
	updateBar();
}

const sf::Vector2f& gui::Scroll::getSize() const
{
	return rect.getSize();
}

sf::FloatRect gui::Scroll::getGlobalBounds() const
{
	return rect.getGlobalBounds();
}

void gui::Scroll::setAt(float x, float y)
{
	if(isHorizontal)setPosition(x, y - getSize().y);
	else setPosition(x - getSize().x, y);
}

void gui::Scroll::setAt(const sf::Vector2f& position)
{
	if (isHorizontal)setPosition(position.x, position.y - getSize().y);
	else setPosition(position.x - getSize().x, position.y);
}

void gui::Scroll::setPosition(float x, float y)
{
	rect.setPosition(x, y);
	updateBar();
}

void gui::Scroll::setPosition(const sf::Vector2f& position)
{
	rect.setPosition(position);
	updateBar();
}

const sf::Vector2f& gui::Scroll::getPosition() const
{
	return rect.getPosition();
}

void gui::Scroll::move(float offsetX, float offsetY)
{
	rect.move(offsetX, offsetY);
}

void gui::Scroll::move(const sf::Vector2f& offset)
{
	rect.move(offset);
}

void gui::Scroll::setHorizontal()
{
	if (!isHorizontal)setSize({ getSize().y, getSize().x });
	isHorizontal = true;
}

void gui::Scroll::setVertical()
{
	if(isHorizontal)setSize({ getSize().y, getSize().x });
	isHorizontal = false;
}

void gui::Scroll::attach(gui::Page** pagePointer)
{
	connectedPage = *pagePointer;
	updateBar();
	connectedPage->setScroll(this);
}

void gui::Scroll::dettach()
{
	bar.setSize(rect.getSize());
	connectedPage = nullptr;
}

void gui::Scroll::scrollTo(const sf::Vector2f& mousePos)
{
	if (isHorizontal) {
		float oldX = bar.getPosition().x;
		float newX = mousePos.x - bar.getSize().x * 0.5f;
		newX = std::max(rect.getPosition().x, newX);
		newX = std::min(rect.getPosition().x +rect.getSize().x - bar.getSize().x, newX);

		bar.setPosition(newX, bar.getPosition().y);

		connectedPage->move(oldX - newX, 0);
	}
	else {
		float oldY = bar.getPosition().y;
		float newY = mousePos.y - bar.getSize().y * 0.5f;
		newY = std::max(rect.getPosition().y, newY);
		newY = std::min(rect.getPosition().y + rect.getSize().y - bar.getSize().y, newY);

		bar.setPosition(bar.getPosition().x, newY);

		connectedPage->move(0, oldY - newY);
	}


}

gui::Clickable* gui::Scroll::isHit(const sf::Vector2f& mousePos)
{
	return bar.getGlobalBounds().contains(mousePos) ? &bar : (rect.getGlobalBounds().contains(mousePos) ? &rect : nullptr);
}

void gui::Scroll::draw(sf::RenderTarget& target)
{
	rect.draw(target);
	bar.draw(target);
}

gui::Scroll* gui::Scroll::create(float height)
{
	return new Scroll(height);
}

gui::Scroll* gui::Scroll::createCopy(const Scroll& scroll)
{
	return new Scroll(scroll);
}
