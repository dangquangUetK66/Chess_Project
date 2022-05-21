#include "mainMenu.h"
#include "gameManager1Player.h"
#include "gameManager2Player.h"

void main() {

	// Game mode
	GameManager gm;
	GameManager2Player gm2;

	// make a main menu
	RenderWindow MENU(VideoMode(504, 504), "Game Menu", Style::Default);
	mainMenu main_Menu(MENU.getSize().x, MENU.getSize().y);

	// set menu background
	
	RectangleShape background;
	background.setSize(Vector2f(504, 504));
	Texture mainTexture;
	mainTexture.loadFromFile("bg/menuBackground.png");
	background.setTexture(&mainTexture);

	// set rule background
	RectangleShape ruleBackground;
	ruleBackground.setSize(Vector2f(504, 504));
	Texture ruleTexture;
	ruleTexture.loadFromFile("bg/ruleBackground.png");
	ruleBackground.setTexture(&ruleTexture);

	// MENU loop
	while (MENU.isOpen())
	{
		Event event;
		while (MENU.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				MENU.close();
			}

			// Press arrow to move menu
			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Up) {
					main_Menu.moveUp();
					break;
				}

				if (event.key.code == Keyboard::Down) {
					main_Menu.moveDown();
					break;
				}

				if (event.key.code == Keyboard::Return) {
					RenderWindow playVsAI(VideoMode(504, 504), "Play vs AI");
					RenderWindow playVsPlay(VideoMode(504, 504), "Play vs Play");
					RenderWindow Rule_About(VideoMode(504, 504), "Rules & About");

					int x = main_Menu.mainMenuPressed();
					if (x == 0) {

						// Play VS AI mode
						while (playVsAI.isOpen())
						{
							Event aevent;
							while (playVsAI.pollEvent(aevent))
							{
								if (aevent.type == Event::Closed)
								{
									playVsAI.close();
								}
							}

							playVsPlay.close();
							Rule_About.close();
							playVsAI.clear();
                            gm.Play(playVsAI);
						}
					}

					if (x == 1) {
						
						// Player vs Player Mode
						while (playVsPlay.isOpen())
						{
							Event aevent;
							while (playVsPlay.pollEvent(aevent))
							{
								if (aevent.type == Event::Closed)
								{
									playVsPlay.close();
								}
							}

							playVsAI.close();
							playVsPlay.clear();
							Rule_About.close();
                            gm2.Play(playVsPlay);

						}
					}

					if (x == 2) {

						// Rule & About
						while (Rule_About.isOpen()) {
							Event aevent;

							while (Rule_About.pollEvent(aevent))
							{
								if (aevent.type == Event::Closed)
								{
									Rule_About.close();
								}

								if (aevent.type == Event::KeyReleased) {
									if (aevent.key.code == Keyboard::Escape) {
										Rule_About.close();
									}
								}
							}

							playVsAI.close();
							playVsPlay.close();
							Rule_About.clear();
							Rule_About.draw(ruleBackground);
							Rule_About.display();

						}
					}

					// Exit
					if (x == 3) {
						MENU.close();
					}
					break;
				}
			}
		}

		MENU.clear();
		MENU.draw(background);
		main_Menu.draw(MENU);
		MENU.display();
	}	
}