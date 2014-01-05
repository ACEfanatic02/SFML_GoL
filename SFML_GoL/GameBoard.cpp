#include "GameBoard.h"

void GameBoard::updateScratch()
{
	static const int scratch_w = m_width + 2;
	static const int scratch_h = m_height + 2;

	sf::Uint8 * src = m_cells;
	sf::Uint8 * dst = m_scratch;

	// Copy src onto dst, leaving the border cells untouched.
	for (int y = 0; y < m_height; ++y) {
		dst += ((y + 1) * scratch_w + 1);
		src += (y * m_width);
		memcpy(dst, src, m_width * sizeof(sf::Uint8));
	}
}

void GameBoard::update()
{
	static const int scratch_w = m_width + 2;
	static const int scratch_h = m_height + 2;

	updateScratch();

	// Cell neighborhood. c maps to the current cell.
	int nw, n, ne,
		w,  c,  e,
		sw, s, se;

	sf::Uint8 * scr = m_scratch;
	sf::Uint8 * dst = m_cells;

	// Cell array is stored in row-major format.  We setup and walk horizontally across each row.
	for(int y = 0; y < m_height; ++y) {
		scr += (y * scratch_w);
		dst += (y * m_width);

		n = scr[0];				ne = scr[1];
		c = scr[scratch_w];		e = scr[scratch_w + 1];
		s = scr[2 * scratch_w]; se = scr[(2 * scratch_w) + 1];

		for (int x = 0; x < m_width; ++x) {

			// NOTES: scr points to nw.
			//        dst[0] is (logically) equivelent to c.
			nw = n; n = ne; ne = scr[2];
			w = c;  c = e;  e = scr[scratch_w + 2];
			sw = s; s = se; se = scr[(2 * scratch_w) + 2];

			// Count live neighbor cells.
			int sum = 
				(nw + n + ne +
				 w  +     e  +
				 sw + s + se);

			if (c == CELL_ALIVE) {
				dst[0] = ((sum == 2 || sum == 3) ? CELL_ALIVE : CELL_DEAD);
			}
			else {
				dst[0] = (sum == 3) ? CELL_ALIVE : CELL_DEAD;
			}

			scr++;
			dst++;
		}
	}
}

GameBoard::CellState GameBoard::cellAt(int x, int y) const
{
	if (checkBounds(x, y)) {
		return m_cells[y * m_width + x];
	}
	// TODO: there are other options here -- we can wrap the board, with cells on one edge neighboring their opposites.
	return CELL_DEAD;
}

GameBoard::CellState GameBoard::cellAt(const sf::Vector2i& pos) const
{
	return cellAt(pos.x, pos.y);
}

void GameBoard::setCell(int x, int y, GameBoard::CellState state)
{
	if (checkBounds(x, y)) {
		m_cells[y * m_width + x] = state;
	}
}

void GameBoard::setCell(const sf::Vector2i& pos, GameBoard::CellState state)
{
	setCell(pos.x, pos.y, state);
}