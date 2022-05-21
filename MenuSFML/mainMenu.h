#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#define max_main_menu 4

class mainMenu
{
	// public
	public:
		mainMenu(float width, float height);
		
		void draw (RenderWindow& window);
		void moveUp();
		void moveDown();
		
		int mainMenuPressed() {
			return mainMenuSelected;
		}
		~mainMenu();

	// private
	private:
		int mainMenuSelected;
		Font font;
		Text mainMenuText[max_main_menu];
};