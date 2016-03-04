#include <windows.h>
#include <tchar.h>
#include <vector>
#include "judge.h"
#include <SFML/Graphics.hpp>

int Convert(int position, bool difference = true){
	if (difference == false)
		return position;
	if (position < kFieldSize / 2)
		return kFieldSize / 2 + position;
	return position - kFieldSize / 2;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
	sf::RenderWindow main_window(sf::VideoMode(1600, 1200), "win");
	sf::Texture black, white;
	black.create(40, 40);
	white.create(40, 40);
	black.loadFromFile("ChipBlack.png");
	white.loadFromFile("ChipWhite.png");
	Judge game(0);
	bool a = 0;
	int moves = 0;
	srand(13213);
	while (main_window.isOpen()){
		sf::Event Event;
		while (main_window.pollEvent(Event)){
			if (Event.type == sf::Event::Closed)
				main_window.close();
			else if (Event.type == sf::Event::MouseWheelMoved){
				if (game.Move()){
					main_window.close();
					break;
				}
			}
		}
		std::vector <int> game_state = game.GetState();
		std::vector<sf::CircleShape> shapes_to_display; 
		for (int i = 0; i < 12; ++i){
			for (int j = 0; j < abs(game_state[i]); ++j){
				sf::CircleShape tm(40);
				if (game_state[i] > 0)
					tm.setTexture(&white);
				else
					tm.setTexture(&black);
				shapes_to_display.push_back(tm);
				shapes_to_display[shapes_to_display.size() - 1].setPosition(sf::Vector2f(50 + i * 100, 1050 - 50 * j));
			}
		}
		for (int i = 12; i < 24; ++i){
			for (int j = 0; j < abs(game_state[i]); ++j){
				sf::CircleShape tm(40);
				if (game_state[i] > 0)
					tm.setTexture(&white);
				else
					tm.setTexture(&black);
				shapes_to_display.push_back(tm);
				shapes_to_display[shapes_to_display.size() - 1].setPosition(sf::Vector2f(50 + (23 - i) * 100, 50 + 50 * j));
			}
		}
		main_window.clear(sf::Color(205, 170, 135));
		for (int j = 0; j < shapes_to_display.size(); ++j)
			main_window.draw(shapes_to_display[j]);
		main_window.display();
	}
	return 0;
}