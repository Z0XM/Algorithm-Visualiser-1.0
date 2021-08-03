#pragma once

#include "Textbox.hpp"
#include "Clickables.hpp"

namespace gui {
	class Button : public Clickable, public Textbox {
	public:
		Button(const sf::Vector2f& siz = sf::Vector2f(0, 0));
		~Button();

		void setHighlightFillColor(const sf::Color& color);
		void setHighlightOutlineColor(const sf::Color& color);
		void setHighlightOutlineThickness(float thickness);

		void setFillColor(const sf::Color& color);
		void setOutlineColor(const sf::Color& color);
		void setOutlineThickness(float thickness);

		bool contains(const sf::Vector2f& mousePos) const;
		Clickable* isHit(const sf::Vector2f& mousePos);

		void activateHighlight();
		void deactivateHighlight();

		void draw(sf::RenderTarget& target) override;

		static Button* create(const sf::Vector2f& size);
		static Button* createCopy(const Button& button);
		static std::unordered_map<std::string, Button*> Group;

	private:
		struct {
			sf::Color fillColor, outlineColor;
			float outlineThickness;
		} highlight, original;
	};
}
