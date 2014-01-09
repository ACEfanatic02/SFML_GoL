#pragma once

#include <SFML/System.hpp>

class GameBoard
{
private:
	const int m_width;
	const int m_height;
	const int scratch_w;
	const int scratch_h;

	// Current state of cells.
	sf::Uint8 * const m_cells;
	// Scratch buffer for cell updates.
	sf::Uint8 * const m_scratch;

	void updateScratch();

	inline bool checkBounds(int x, int y) const 
	{
		return (x >= 0 && x < m_width && y >= 0 && y < m_height);
	}

public:
	
	typedef sf::Uint8 CellState;
	static const CellState CELL_DEAD = 0;
	static const CellState CELL_ALIVE = 1;

	GameBoard(int width, int height) : 
		m_width(width),
		m_height(height),
		scratch_w(width+2),
		scratch_h(height+2),
		m_cells(new sf::Uint8[width * height]),
		m_scratch(new sf::Uint8[scratch_w * scratch_h])
	{
		for (int x = 0; x < width; ++x) {
			for (int y = 0; y < height; ++y) {
				m_cells[y * width + x] = CELL_ALIVE;
			}
		}

		for (int x = 0; x < (width + 2); ++x) {
			for (int y = 0; y < (height + 2); ++y) {
				m_scratch[y * (width + 2) + x] = CELL_DEAD;
			}
		}
	}

	~GameBoard()
	{
		delete[] m_cells;
		delete[] m_scratch;
	}

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }

	void update();

	CellState cellAt(int x, int y) const;
	CellState cellAt(const sf::Vector2i& pos) const;

	void setCell(int x, int y, CellState state);
	void setCell(const sf::Vector2i& pos, CellState state);
};

