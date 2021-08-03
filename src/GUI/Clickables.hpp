#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

namespace gui {
	class Clickable {
	protected:
		enum { HIGHLIGHT, ORIGINAL } state;

	public:
		Clickable();
		virtual ~Clickable();

		virtual void activateHighlight() = 0;
		virtual void deactivateHighlight() = 0;
		virtual Clickable* isHit(const sf::Vector2f& mousePos) = 0;

		void setAction(std::function<void()> func);
		std::function<void()> action;

		static Clickable* mouseHoveringOn;
		static sf::Mouse::Button pressedMouseButton;
		static Clickable* clicked;

		enum { UNREGISTER, REGISTER, MOUSEMOVE } actionEvent;
	};
}