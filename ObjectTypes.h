#include <map>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
struct Token {
	sf::CircleShape token;
	int x;
	int y;
	int timestouched = 0;
	bool debounce = true;
	sf::Color tokencolor = sf::Color::Yellow;
	void draw(sf::RenderWindow* window) {
		token.setFillColor(tokencolor);
		token.setRadius(8);
		token.setPosition(x * 16, y * 16);
		window->draw(token);
	}
};
struct Player {
	sf::CircleShape player;
	int x;
	int y;
	void draw(sf::RenderWindow* window) {
		player.setFillColor(sf::Color::Red);
		player.setRadius(8);
		player.setPosition(x * 16, y * 16);
		window->draw(player);
	}
	Player(int x1, int y1) {
		x = x1;
		y = y1;
	}
};