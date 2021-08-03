#pragma once

#include "Clickables.hpp"
#include "Textbox.hpp"
#include <SFML/Graphics.hpp>

namespace gui {
	class Slider : public Clickable, public Textbox {
	private:
		sf::RectangleShape bar;
		sf::Color barColor, barHighlight;
		float offset;
		
		void updateBar();
	public:
		Slider(const sf::Vector2f& size);
		~Slider();

		float getMappedOffset()const;

		void setBarFillColor(sf::Color color);
		void setBarHighlightColor(sf::Color color);

		void setSize(const sf::Vector2f& size);
		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);
		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f& offset);

		void reset();
		void moveSliderBar(const sf::Vector2f& mousePos);
		Clickable* isHit(const sf::Vector2f& mousePos);

		void activateHighlight();
		void deactivateHighlight();

		void draw(sf::RenderTarget& target) override;

		static Slider* create(const sf::Vector2f& size);
		static Slider* createCopy(const Slider& slider);
		static std::unordered_map<std::string, Slider*> Group;
	};
}