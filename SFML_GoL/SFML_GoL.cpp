// SFML_GoL.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "ToolBar.h"

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef DEBUG
	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
#endif

	sf::RenderWindow window(sf::VideoMode(800, 832), "Game Of Life");

	sf::Texture tiles;
	if (!tiles.loadFromFile("tiles.png")) {
		sf::err() << "Failed to load tileset texture." << std::endl;
		return 1;
	}
	sf::Texture toolbar_tex;
	if (!toolbar_tex.loadFromFile("toolbar.png")) {
		sf::err() << "Failed to load toolbar texture." << std::endl;
		return 1;
	}
	
	GameState board(sf::Vector2u(100, 100), sf::Vector2u(8, 8), tiles);
	ToolBar toolbar(toolbar_tex);
	toolbar.setPosition(0, 800);

	toolbar.bindFunction(1, [&board]() {
		board.setSpeed(GameSpeed::SPEED_PAUSED);
	});
	toolbar.bindFunction(2, [&board]() {
		board.setSpeed(GameSpeed::SPEED_SLOW);
	});
	toolbar.bindFunction(3, [&board]() {
		board.setSpeed(GameSpeed::SPEED_FAST);
	});
	toolbar.bindFunction(5, [&board]() {
		board.rotateBrush();
	});
	toolbar.bindFunction(7, [&board]() {
		board.setBrush(ToolBrushes::SINGLE_CELL);
	});
	toolbar.bindFunction(8, [&board]() {
		board.setBrush(ToolBrushes::BLOCK);
	});
	toolbar.bindFunction(9, [&board]() {
		board.setBrush(ToolBrushes::GLIDER);
	});
	toolbar.bindFunction(10, [&board]() {
		board.setBrush(ToolBrushes::BLINKER);
	});

	sf::Clock clock;
	sf::Vector2i click;

	sf::View game_view(sf::Vector2f(400, 400), sf::Vector2f(800, 800));
	game_view.setViewport(sf::FloatRect(0, 0, 1, 800.0 / 832.0));
	window.setView(game_view);
	window.setMouseCursorVisible(false);
	window.setFramerateLimit(60);

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
				if (event.mouseButton.y >= 800) {
					// User clicked on toolbar
					toolbar.click(sf::Vector2i(event.mouseButton.x, event.mouseButton.y - 800));
				}
				else {
					if (event.mouseButton.button == sf::Mouse::Button::Left) { 
						board.stamp();
					} else if (event.mouseButton.button == sf::Mouse::Button::Right) {
						board.erase();
					}
				}
				break;
			case sf::Event::MouseMoved:
				sf::Vector2i pos = sf::Mouse::getPosition(window);
				if (pos.y >= 800) {
					toolbar.mouseover(pos - sf::Vector2i(0, 800));
					window.setMouseCursorVisible(true);
					board.setBrushVisible(false);
				}
				else {
					window.setMouseCursorVisible(false);
					toolbar.clearSelection();
					board.setBrushPosition(sf::Vector2i(window.mapPixelToCoords(pos)));
					board.setBrushVisible(true);
				}
				break;
			}
		}
		
		{
			// Do not show brush if cursor is outside our window.
			sf::FloatRect window_bounds(sf::Vector2f(window.getPosition()), sf::Vector2f(window.getSize()));
			if (!window_bounds.contains(sf::Vector2f(sf::Mouse::getPosition())))
			{
				board.setBrushVisible(false);
			}
		}

		sf::Time elapsed = clock.restart();
		board.update(elapsed);
		toolbar.update();

		window.clear(sf::Color::Blue);
		window.draw(board);
		window.setView(window.getDefaultView());
		window.draw(toolbar);
		window.setView(game_view);
		window.display();
	}
	return 0;
}

