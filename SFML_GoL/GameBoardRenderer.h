#pragma once

#include <SFML/Graphics.hpp>

#include "GameBoard.h"

class GameBoardRenderer : public sf::Drawable
{
private:
	GameBoard m_board;
	const sf::Vector2u m_cellsize;
	const sf::Texture& m_tex;
	sf::VertexArray m_verts;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	GameBoardRenderer(const sf::Vector2u& board_size, const sf::Vector2u& cellsize, const sf::Texture& texture) :
		m_board(board_size.x, board_size.y),
		m_cellsize(cellsize),
		m_tex(texture),
		m_verts(sf::Quads, board_size.x * board_size.y * 4)
	{
	}

	~GameBoardRenderer()
	{
	}

	void update();
	void clickCell(const sf::Vector2i& pos);
};

