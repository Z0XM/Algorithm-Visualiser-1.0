#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>
#include "Button.hpp"
#include "Textbox.hpp"
#include "Slider.hpp"
#include "Clickables.hpp"

namespace gui {
	class Scroll;
	class Page {
	private:
		sf::RectangleShape box;
		Scroll* scroll;
		bool active;
	public:
		Page(const sf::Vector2f& size = sf::Vector2f(0, 0));
		~Page();

		void setSize(const sf::Vector2f& size);
		const sf::Vector2f& getSize() const;

		void setFillColor(const sf::Color& color);
		void setOutlineColor(const sf::Color& color);
		void setOutlineThickness(float thickness);

		const sf::Color& getFillColor() const;
		const sf::Color& getOutlineColor() const;
		float getOutlineThickness() const;

		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);
		const sf::Vector2f& getPosition() const;

		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f& offset);

		Clickable* isHit(const sf::Vector2f& mousePos);

		void setScroll(Scroll* scroll);
		void setActive();
		void setInactive();

		std::function<sf::Vector2f()> getMousePositionInPage;
		void setMousePositionFunction(std::function<sf::Vector2f()> func);

		virtual void draw(sf::RenderTarget&);

		static Page* create(const sf::Vector2f& size);
		static Page* createCopy(const Page& page);

		std::unordered_map<std::string, Button*> Buttons;
		std::unordered_map<std::string, Textbox*> Textboxes;
		std::unordered_map<std::string, Slider*> Sliders;
		static std::unordered_map<std::string, Page*> Group;
	};
}