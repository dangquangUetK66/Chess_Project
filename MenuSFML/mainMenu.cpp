#include "mainMenu.h"

mainMenu::mainMenu(float width, float height)
{
	if (!font.loadFromFile("Font//Raleway-Regular.ttf"))
	{
		cout << "Error loading font" << endl;
	}

	// Player vs Computer
	
	mainMenuText[0].setFont(font);
	mainMenuText[0].setFillColor(Color::White);
	mainMenuText[0].setString("Player vs Computer");
	mainMenuText[0].setCharacterSize(30);
	mainMenuText[0].setPosition(100, 175);

	// Player vs Player
	
	mainMenuText[1].setFont(font);
	mainMenuText[1].setFillColor(Color::White);
	mainMenuText[1].setString("Player vs Player");
	mainMenuText[1].setCharacterSize(30);
	mainMenuText[1].setPosition(100, 250);
	
	// Rules & About
	
	mainMenuText[2].setFont(font);
	mainMenuText[2].setFillColor(Color::White);
	mainMenuText[2].setString("Rules & About");
	mainMenuText[2].setCharacterSize(30);
	mainMenuText[2].setPosition(100, 325);
	
	// Exit
	
	mainMenuText[3].setFont(font);
	mainMenuText[3].setFillColor(Color::White);
	mainMenuText[3].setString("Exit");
	mainMenuText[3].setCharacterSize(30);
	mainMenuText[3].setPosition(100, 400);

	mainMenuSelected = -1;
}

mainMenu:: ~mainMenu()
{
	
}

// Draws the main menu
void mainMenu::draw(RenderWindow& window) {
	for (int i = 0; i < max_main_menu; i++) {
		window.draw(mainMenuText[i]);
	}
}

// Move up using arrow
void mainMenu::moveUp() {
	if (mainMenuSelected - 1 >= 0) {
		mainMenuText[mainMenuSelected].setFillColor(Color::White);

		mainMenuSelected--;
		if (mainMenuSelected == -1) {
			mainMenuSelected = 2;
		}
		mainMenuText[mainMenuSelected].setFillColor(Color::Blue);
	}
}

// Move down using arrow
void mainMenu::moveDown() {
	if (mainMenuSelected + 1 <= max_main_menu) {
		mainMenuText[mainMenuSelected].setFillColor(Color::White);

		mainMenuSelected++;
		if (mainMenuSelected == max_main_menu) {
			mainMenuSelected = 0;
		}
		mainMenuText[mainMenuSelected].setFillColor(Color::Blue);
	}
}