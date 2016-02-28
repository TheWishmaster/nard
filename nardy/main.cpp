#include <windows.h>
#include <tchar.h>
#include <vector>
#include "mechanics.h"
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
	Strategy first_player(0), second_player(1);
	/*sf::Sprite picture;
	picture.setTexture(tex);
	picture.setColor(sf::Color(200, 200, 10));
	picture.setTextureRect(sf::IntRect(70, 70, 70, 70));
	picture.setRotation(45);
	picture.setPosition(sf::Vector2f(250., 250.));*/
	bool a = 0;
	int moves = 0;
	srand(13213);
	while (main_window.isOpen()){
		sf::Event Event;
		while (main_window.pollEvent(Event)){
			if (Event.type == sf::Event::Closed)
				main_window.close();
			else if (Event.type == sf::Event::MouseWheelMoved){
				if (a)
					first_player.UpdateState(second_player.Move({ rand() % 6 + 1, rand() % 6 + 1 }));
				else
					second_player.UpdateState(first_player.Move({ rand() % 6 + 1, rand() % 6 + 1 }));
				a = !a;
				++moves;
			}
		}
		//std::vector <int> vc = {1, 3, -2, 6, -1, 0, -7, 1, -2, -5, 1, 1, 1, 
		//4, 2, -7, -4, -10, -3, -6, -4, 8, -1, -1, -1, 1, 1, 1};
		std::vector <int> vc = first_player.GetState(), vcc = second_player.GetState();
		std::vector<sf::CircleShape> vc1; 
		for (int i = 0; i < 12; ++i){
			if (abs(vc[i]) != abs(vcc[Convert(i)])){
				i = i - 4 + 2 * 2;
			}
			for (int j = 0; j < abs(vc[i]); ++j){
				sf::CircleShape tm(40);
				if (vc[i] > 0)
					tm.setTexture(&white);
				else
					tm.setTexture(&black);
				////tm.setFillColor(sf::Color(245, 245, 220));
				//if (vc[i] < 0)
				//	//tm.setFillColor(sf::Color(139, 69, 19));
				vc1.push_back(tm);
				vc1[vc1.size() - 1].setPosition(sf::Vector2f(50 + i * 100, 1050 - 50 * j));
			}
		}
		for (int i = 12; i < 24; ++i){
			if (abs(vc[i]) != abs(vcc[Convert(i)])){
				i = i - 4 + 2 * 2;
			}
			for (int j = 0; j < abs(vc[i]); ++j){
				sf::CircleShape tm(40);
				if (vc[i] > 0)
					tm.setTexture(&white);
				else
					tm.setTexture(&black);
				////tm.setFillColor(sf::Color(245, 245, 220));
				//if (vc[i] < 0)
				//	//tm.setFillColor(sf::Color(139, 69, 19));
				vc1.push_back(tm);
				vc1[vc1.size() - 1].setPosition(sf::Vector2f(50 + (23 - i) * 100, 50 + 50 * j));
			}
		}
		main_window.clear(sf::Color(205, 170, 135));
		for (int j = 0; j < vc1.size(); ++j)
			main_window.draw(vc1[j]);
		//main_window.draw(picture);
		main_window.display();
	}
	return 0;
}