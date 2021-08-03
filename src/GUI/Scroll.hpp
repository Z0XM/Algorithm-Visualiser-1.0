#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Button.hpp"
#include "Page.hpp"
#include <vector>
#include <functional>

namespace gui {
	class Scroll {
	private:
		Button bar, rect;
		bool isHorizontal;
		
		void updateBar();

		Page* connectedPage;
	public:
		Scroll(float height);
		~Scroll();

		void setSize(const sf::Vector2f& size);
		const sf::Vector2f& getSize() const;

		sf::FloatRect getGlobalBounds() const;

		void setAt(float x, float y);
		void setAt(const sf::Vector2f& position);
	
		void setPosition(float x, float y);
		void setPosition(const sf::Vector2f& position);
		const sf::Vector2f& getPosition() const;

		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f& offset);

		void setHorizontal();
		void setVertical();

		void attach(Page** page);
		void dettach();

		void scrollTo(const sf::Vector2f& mousePos);

		Clickable* isHit(const sf::Vector2f& mousePos);

		void draw(sf::RenderTarget& target);

		std::function<sf::Vector2f()> getMousePosition;

		static Scroll* create(float height);
		static Scroll* createCopy(const Scroll& scroll);
		static std::unordered_map<std::string, Scroll*> Group;
	};
}
