#pragma once

#include <SFML/Graphics.hpp>

#include "GameBoard.h"
#include "ToolBrushRenderer.h"

enum GameSpeed
{
	SPEED_PAUSED,
	SPEED_SLOW,
	SPEED_FAST,
};

class GameState : public sf::Drawable
{
private:
	GameBoard m_board;
	const sf::Vector2u m_cellsize;
	const sf::Texture& m_tex;
	sf::VertexArray m_verts;

	sf::Vector2i m_brush_pos;

	ToolBrushRenderer m_toolbrush;

	GameSpeed m_speed;
	sf::Time m_time_since_update;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool checkUpdateTime(sf::Time elapsed);
	void updateBrush();
	void stampBrush(GameBoard::CellState state);

public:
	GameState(const sf::Vector2u& board_size, const sf::Vector2u& cellsize, const sf::Texture& texture) :
		m_board(board_size.x, board_size.y),
		m_cellsize(cellsize),
		m_tex(texture),
		m_verts(sf::Quads, board_size.x * board_size.y * 4),
		m_speed(GameSpeed::SPEED_PAUSED),
		m_toolbrush(&texture, cellsize),
		m_brush_pos(0, 0)
	{
	}
	
	~GameState()
	{
	}

	// Update the game by given elapsed time.
	void update(const sf::Time elapsed);
	// Stamp brush to board with live cells.
	void stamp();
	// Stamp brush to board with dead cells.
	void erase();
	// Set game speed.
	void setSpeed(const GameSpeed speed) { m_speed = speed; }
	// Set current active brush.
	void setBrush(const ToolBrush& brush) { m_toolbrush.setBrush(brush); }
	// Set brush visibility.
	void setBrushVisible(bool visible) { m_toolbrush.setVisible(visible); }
	// Set brush position (pixel coordinates, relative to upper-left of game board).
	void setBrushPosition(const sf::Vector2i& pos);
	// Rotate brush.
	void rotateBrush() { m_toolbrush.rotateBrush(); }
};

