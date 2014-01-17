// SFML_GoL.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "GameBoardRenderer.h"

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
#endif

	sf::RenderWindow window(sf::VideoMode(800, 800), "Game Of Life");

	sf::Texture tiles;
	if (!tiles.loadFromFile("tiles.png")) {
		sf::err() << "Failed to load tileset texture." << std::endl;
		return 1;
	}
	
	GameBoardRenderer board(sf::Vector2u(100, 100), sf::Vector2u(8, 8), tiles);

	sf::Clock clock;
	sf::Vector2i click;

	sf::View game_view(sf::Vector2f(400, 400), sf::Vector2f(800, 800));
	game_view.setViewport(sf::FloatRect(0, 0, 1, 1));
	window.setView(game_view);
	window.setMouseCursorVisible(false);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				// Re-center view
				case sf::Keyboard::C:
					game_view.setCenter(sf::Vector2f(400, 400));
					window.setView(game_view);
					break;

				// Change speed:
				case sf::Keyboard::P:
					board.setSpeed(GameSpeed::SPEED_PAUSED);
					break;
				case sf::Keyboard::Num1:
					board.setSpeed(GameSpeed::SPEED_SLOW);
					break;
				case sf::Keyboard::Num2:
					board.setSpeed(GameSpeed::SPEED_FAST);
					break;

				// Switch brush:
				case sf::Keyboard::Numpad0:
					board.setBrush(ToolBrushes::SINGLE_CELL);
					break;
				case sf::Keyboard::Numpad1:
					board.setBrush(ToolBrushes::BLOCK);
					break;
				case sf::Keyboard::Numpad2:
					board.setBrush(ToolBrushes::GLIDER);
					break;
				case sf::Keyboard::Numpad3:
					board.setBrush(ToolBrushes::BLINKER);
					break;

				// Rotate brush;
				case sf::Keyboard::R:
					board.rotateBrush();
					break;
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Button::Left) { 
					board.stamp();
				} else if (event.mouseButton.button == sf::Mouse::Button::Right) {
					board.erase();
				}
				break;
			case sf::Event::MouseMoved:
				sf::Vector2i pos = sf::Mouse::getPosition(window);		
				board.setBrushPosition(sf::Vector2i(window.mapPixelToCoords(pos)));
				break;
			}
		}
		
		sf::Time elapsed = clock.restart();
		board.update(elapsed);

		window.clear(sf::Color::Blue);
		window.draw(board);
		window.display();
		sf::sleep(sf::milliseconds(50) - elapsed);
	}
	return 0;
}

