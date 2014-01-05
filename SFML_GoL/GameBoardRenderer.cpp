#include "GameBoardRenderer.h"

void GameBoardRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_verts, states);
}

void GameBoardRenderer::update()
{
	m_board.update();

	const int tileset_width = m_tex.getSize().x / m_cellsize.x;
	for (unsigned int i = 0; i < m_board.getWidth(); ++i)
	{
		for (unsigned int j = 0; j < m_board.getHeight(); ++j)
		{
			int celltype = m_board.cellAt(i, j);

			int tu = celltype % tileset_width;
			int tv = celltype / tileset_width;

			sf::Vertex * quad = &m_verts[(i + m_board.getWidth() * j) * 4];
			
			quad[0].position = sf::Vector2f(i * m_cellsize.x, j * m_cellsize.y);
			quad[1].position = sf::Vector2f((i + 1) * m_cellsize.x, j * m_cellsize.y);
			quad[2].position = sf::Vector2f((i + 1) * m_cellsize.x, (j + 1) * m_cellsize.y);
			quad[3].position = sf::Vector2f(i * m_cellsize.x, (j + 1) * m_cellsize.y);

			quad[0].texCoords = sf::Vector2f(tu * m_cellsize.x, tv * m_cellsize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * m_cellsize.x, tv * m_cellsize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * m_cellsize.x, (tv + 1) * m_cellsize.y);
			quad[3].texCoords = sf::Vector2f(tu * m_cellsize.x, (tv + 1) * m_cellsize.y);	
		}
	}
}

void GameBoardRenderer::clickCell(const sf::Vector2i& pos)
{
	int tile_x = pos.x / m_cellsize.x;
	int tile_y = pos.y / m_cellsize.y;
	
	int cur_state = m_board.cellAt(tile_x, tile_y);
	m_board.setCell(tile_x, tile_y, (~cur_state) & 0x1); 
}