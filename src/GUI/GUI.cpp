#include "../GUI.hpp"
#include <iostream>
#include <unordered_map>
#include "Clickables.hpp"

namespace gui {

	template <typename T>
	void renderGroup(std::unordered_map<std::string, T*>& map, sf::RenderTarget& renderTarget)
	{
		for (auto it = map.begin(); it != map.end(); it++) {
			it->second->draw(renderTarget);
		}
	}

	void render(sf::RenderTarget& renderTarget)
	{
		renderGroup(Page::Group, renderTarget);
		renderGroup(Scroll::Group, renderTarget);
		renderGroup(Textbox::Group, renderTarget);
		renderGroup(Button::Group, renderTarget);
		renderGroup(Slider::Group, renderTarget);
	}

	template <typename T>
	void deleteGroup(std::unordered_map<std::string, T*>& map)
	{
		for (auto it = map.begin(); it != map.end(); it++) {
			delete it->second;
		}
		map.clear();
	}

	void deleteAllGuiGroups()
	{
		deleteGroup(Page::Group);
		deleteGroup(Scroll::Group);
		deleteGroup(Textbox::Group);
		deleteGroup(Button::Group);
		deleteGroup(Slider::Group);
	}

	template<typename T>
	gui::Clickable* searchForPointedButtonIn(std::unordered_map<std::string, T*>& map, const sf::Vector2f& mousePos)
	{
		for (auto it = map.begin(); it != map.end(); it++) {
			Clickable* hit = it->second->isHit(mousePos);
			if (hit != nullptr)return hit;
		}
		return nullptr;
	}

	void updateMousePointer(const sf::Vector2f& mousePos)
	{
		if (Clickable::clicked != nullptr && Clickable::clicked->actionEvent == Clickable::MOUSEMOVE && Clickable::clicked->action != nullptr) {
			Clickable::clicked->action();
		}
		else {
			Clickable* currentMouseHoveringOn = nullptr;

			currentMouseHoveringOn = searchForPointedButtonIn(Page::Group, mousePos);
			if (currentMouseHoveringOn == nullptr)
				currentMouseHoveringOn = searchForPointedButtonIn(Scroll::Group, mousePos);
			if (currentMouseHoveringOn == nullptr)
				currentMouseHoveringOn = searchForPointedButtonIn(Button::Group, mousePos);
			if (currentMouseHoveringOn == nullptr)
				currentMouseHoveringOn = searchForPointedButtonIn(Slider::Group, mousePos);

			if (Clickable::mouseHoveringOn != currentMouseHoveringOn)
			{
				if (Clickable::mouseHoveringOn != nullptr)Clickable::mouseHoveringOn->deactivateHighlight();
				Clickable::mouseHoveringOn = currentMouseHoveringOn;
				if (Clickable::mouseHoveringOn != nullptr)Clickable::mouseHoveringOn->activateHighlight();
			}
		}
	}

	void registerClick(sf::Mouse::Button button)
	{
		Clickable::clicked = Clickable::mouseHoveringOn;
		Clickable::pressedMouseButton = button;

		if (Clickable::clicked != nullptr && Clickable::clicked->actionEvent == Clickable::REGISTER && Clickable::clicked->action != nullptr)
			Clickable::clicked->action();
	}

	void unregisterClick(sf::Mouse::Button button, const sf::Vector2f& mousePos)
	{
		if (Clickable::clicked != nullptr && Clickable::clicked == Clickable::mouseHoveringOn && button == Clickable::pressedMouseButton)
		{
			if(Clickable::clicked->actionEvent == Clickable::UNREGISTER && Clickable::clicked->action != nullptr)
				Clickable::clicked->action();
		}
		Clickable::clicked = nullptr;
	}
}