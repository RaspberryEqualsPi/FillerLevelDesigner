#pragma once
#include <map>
#include <string>
#include "ArcadeTTF.h"
#include <SFML/Window.hpp>
#include "ArrowShooterPNG.h"
#include <SFML/Graphics.hpp>
#include <windows.h>
#include "RoundedRectangle.h"
bool appInFocus(sf::RenderWindow* app)
{
	if (app == NULL)
		return false;

	HWND handle = app->getSystemHandle();
	bool one = handle == GetFocus();
	bool two = handle == GetForegroundWindow();

	if (one != two) //strange 'half-focus': window is in front but you can't click anything - so we fix it
	{
		SetFocus(handle);
		SetForegroundWindow(handle);
	}

	return one && two;
}

namespace FillerUI {
	bool clicking = false;
	bool rclicking = false;
	bool StartPos = false;
	bool ArrowShooter = false;
	int globalAtype = 0;
	void SetClicking(bool state) {
		clicking = state;
	}
	void SetRightClicking(bool state) {
		rclicking = state;
	}
	struct TextButton {
		int cornerPoints = 10;
		sf::Color textColor = sf::Color::White;
		int textSize = 8;
		std::string text;
		float sizeX = 90;
		float sizeY = 32;
		float radius = sizeX / 10;
		sf::Text sfText;
		sf::Font sfFont;
		sf::RoundedRectangleShape rect = sf::RoundedRectangleShape(sf::Vector2f(sizeX, sizeY), radius, cornerPoints);
		void draw(sf::RenderWindow* window) {
			rect.setSize(sf::Vector2f(sizeX, sizeY));
			rect.setCornerPointCount(cornerPoints);
			rect.setCornersRadius(radius);
			rect.setFillColor(sf::Color::Blue);
			//sfFont.loadFromFile(font);
			sfFont.loadFromMemory(ArcadeTTF, ArcadeTTF_length);
			sfText.setFont(sfFont);
			sfText.setString(text);
			sfText.setCharacterSize(textSize);
			sfText.setFillColor(textColor);
			float textWidth = sfText.getGlobalBounds().width;
			float textHeight = sfText.getGlobalBounds().height;
			float buttonWidth = rect.getGlobalBounds().width;
			float buttonHeight = rect.getGlobalBounds().height;
			float buttonX = rect.getPosition().x;
			float buttonY = rect.getPosition().y;
			sfText.setPosition(((buttonWidth - textWidth) / 2) + buttonX, ((buttonHeight - textHeight) / 2) + buttonY);
			window->draw(rect);
			window->draw(sfText);
		}
		bool clicked(sf::RenderWindow* window) {
			auto mouse_pos = sf::Mouse::getPosition(*window);
			auto translated_pos = window->mapPixelToCoords(mouse_pos);
			return rect.getGlobalBounds().contains(translated_pos) && clicking && appInFocus(window);
		}
	};
	struct RectButton {
		sf::RectangleShape rect;
		float x = 0;
		float y = 0;
		bool state = true;
		void draw(sf::RenderWindow* window) {
			rect.setSize(sf::Vector2f(16, 16));
			rect.setFillColor(sf::Color::White);
			rect.setPosition(x * 16, y * 16);
			auto mouse_pos = sf::Mouse::getPosition(*window);
			auto translated_pos = window->mapPixelToCoords(mouse_pos);
			if (rect.getGlobalBounds().contains(translated_pos) && appInFocus(window)) {
				sf::RectangleShape outline;
				outline.setFillColor(sf::Color::Black);
				outline.setPosition(rect.getPosition());
				outline.setSize(sf::Vector2f(16, 16));
				rect.setSize(sf::Vector2f(12, 12));
				rect.setPosition(x * 16 + 2, y * 16 + 2);
				window->draw(outline);
			}
			window->draw(rect);
		}
		bool clicked(sf::RenderWindow* window) {
			auto mouse_pos = sf::Mouse::getPosition(*window);
			auto translated_pos = window->mapPixelToCoords(mouse_pos);
			return rect.getGlobalBounds().contains(translated_pos) && clicking && appInFocus(window);
		}
	};
	struct CircleButton {
		sf::CircleShape circle;
		float x = 0;
		float y = 0;
		int aType = 0;
		bool arrowstate = false;
		bool state = false;
		bool startPos = false;
		bool checkpointstate = false;
		sf::Texture Tarrowshooter;
		sf::Sprite Sarrowshooter;
		void draw(sf::RenderWindow* window) {
			circle.setRadius(8);
			if(!startPos)
				circle.setFillColor(sf::Color::Yellow);
			if(startPos)
				circle.setFillColor(sf::Color::Red);
			if (checkpointstate)
				circle.setFillColor(sf::Color(0, 255, 0));
			circle.setPosition(x * 16, y * 16);
			auto mouse_pos = sf::Mouse::getPosition(*window);
			auto translated_pos = window->mapPixelToCoords(mouse_pos);
			if (circle.getGlobalBounds().contains(translated_pos) && appInFocus(window) && !StartPos) {
				circle.setFillColor(sf::Color::Blue);
			}
			if (circle.getGlobalBounds().contains(translated_pos) && appInFocus(window) && StartPos) {
				circle.setFillColor(sf::Color::Red);
			}
			if (arrowstate) {
				Tarrowshooter.loadFromMemory(ArrowShooterPNG, ArrowShooterPNG_length);
				Sarrowshooter.setTexture(Tarrowshooter);
				Sarrowshooter.setOrigin(0, 0);
				if (aType == 1) {
					Sarrowshooter.setPosition(x * 16, y * 16 + 16);
				}
				if (aType == 2) {
					Sarrowshooter.setPosition(x * 16, y * 16);
				}
				if (aType == 3) {
					Sarrowshooter.setPosition(x * 16 + 16, y * 16);
				}
				if (aType == 0) {
					Sarrowshooter.setPosition(x * 16 + 16, y * 16 + 16);
				}
				Sarrowshooter.setOrigin(Sarrowshooter.getGlobalBounds().width, Sarrowshooter.getGlobalBounds().height);
				Sarrowshooter.setRotation(aType * 90);
				window->draw(Sarrowshooter);
			}
			if (circle.getGlobalBounds().contains(translated_pos) && appInFocus(window) && ArrowShooter) {
				Tarrowshooter.loadFromMemory(ArrowShooterPNG, ArrowShooterPNG_length);
				Sarrowshooter.setTexture(Tarrowshooter);
				Sarrowshooter.setOrigin(0, 0);
				if (globalAtype == 1) {
					Sarrowshooter.setPosition(x * 16, y * 16 + 16);
				}
				if (globalAtype == 2) {
					Sarrowshooter.setPosition(x * 16, y * 16);
				}
				if (globalAtype == 3) {
					Sarrowshooter.setPosition(x * 16 + 16, y * 16);
				}
				if (globalAtype == 0) {
					Sarrowshooter.setPosition(x * 16 + 16, y * 16 + 16);
				}
				Sarrowshooter.setOrigin(Sarrowshooter.getGlobalBounds().width, Sarrowshooter.getGlobalBounds().height);
				Sarrowshooter.setRotation(globalAtype * 90);
				window->draw(Sarrowshooter);
			}
			if(state && !arrowstate && !(circle.getGlobalBounds().contains(translated_pos) && ArrowShooter))
				window->draw(circle);
		}
		bool clicked(sf::RenderWindow* window) {
			auto mouse_pos = sf::Mouse::getPosition(*window);
			auto translated_pos = window->mapPixelToCoords(mouse_pos);
			return circle.getGlobalBounds().contains(translated_pos) && clicking && appInFocus(window);
		}
		bool rclicked(sf::RenderWindow* window) {
			auto mouse_pos = sf::Mouse::getPosition(*window);
			auto translated_pos = window->mapPixelToCoords(mouse_pos);
			return circle.getGlobalBounds().contains(translated_pos) && rclicking && appInFocus(window);
		}
	};
}