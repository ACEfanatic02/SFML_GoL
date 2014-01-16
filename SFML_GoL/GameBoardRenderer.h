#pragma once

#include <SFML/Graphics.hpp>

#include "GameBoard.h"
#include "ToolBrush.h"

enum GameSpeed
{
	SPEED_PAUSED,
	SPEED_SLOW,
	SPEED_FAST,
};

class GameBoardRenderer : public sf::Drawable
{
private:
	GameBoard m_board;
	const sf::Vector2u m_cellsize;
	const sf::Texture& m_tex;
	sf::VertexArray m_verts;

	const ToolBrush * m_brush;
	sf::Vector2i m_brush_pos;
	sf::VertexArray m_brush_verts;

	GameSpeed m_speed;
	sf::Time m_time_since_update;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool checkUpdateTime(sf::Time elapsed);
	void clearBrushVerts();
	void updateBrush();

public:
	GameBoardRenderer(const sf::Vector2u& board_size, const sf::Vector2u& cellsize, const sf::Texture& texture) :
		m_board(board_size.x, board_size.y),
		m_cellsize(cellsize),
		m_tex(texture),
		m_verts(sf::Quads, board_size.x * board_size.y * 4),
		m_speed(GameSpeed::SPEED_PAUSED),
		m_brush(&ToolBrushes::SINGLE_CELL),
		m_brush_pos(0, 0),
		m_brush_verts(sf::VertexArray(sf::Quads, 4 * 3 * 3))
	{
	}
	
	~GameBoardRenderer()
	{
	}

	void update(const sf::Time elapsed);
	void stampBrush();
	void eraseBrush();
	void setSpeed(const GameSpeed speed) { m_speed = speed; }
	void setBrush(const ToolBrush& brush) { m_brush = &brush; clearBrushVerts(); }
	void setBrushPosition(const sf::Vector2i& pos);
};

