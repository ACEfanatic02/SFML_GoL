#include "GameBoardRenderer.h"

void GameBoardRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &m_tex;
	target.draw(m_verts, states);
	target.draw(m_brush_verts, states);
}

namespace {
	const sf::Time SLOW_TIMEOUT = sf::milliseconds(300);
	const sf::Time FAST_TIMEOUT = sf::milliseconds(100);
}

void GameBoardRenderer::updateBrush()
{
	const int b_width = m_brush->width;
	const int b_height = m_brush->height;
	for (int x = 0; x < b_width; ++x)
	{
		for (int y = 0; y < b_height; ++y)
		{
			sf::Vertex * quads = &m_brush_verts[(y * b_width + x) * 4];
			
			quads[0].position = sf::Vector2f(x * m_cellsize.x, y * m_cellsize.y) + sf::Vector2f(m_brush_pos);
			quads[1].position = sf::Vector2f((x + 1) * m_cellsize.x, y * m_cellsize.y) + sf::Vector2f(m_brush_pos);
			quads[2].position = sf::Vector2f((x + 1) * m_cellsize.x, (y + 1) * m_cellsize.y) + sf::Vector2f(m_brush_pos);
			quads[3].position = sf::Vector2f(x * m_cellsize.x, (y + 1) * m_cellsize.y) + sf::Vector2f(m_brush_pos);

			if (m_brush->brush[y * b_width + x])
			{
				quads[0].texCoords = sf::Vector2f(0.0f, 8.0f);
				quads[1].texCoords = sf::Vector2f(8.0f, 8.0f);
				quads[2].texCoords = sf::Vector2f(8.0f, 16.0f);
				quads[3].texCoords = sf::Vector2f(0.0f, 16.0f);
			}
			else
			{
				quads[0].color = sf::Color::Transparent;
				quads[1].color = sf::Color::Transparent;
				quads[2].color = sf::Color::Transparent;
				quads[3].color = sf::Color::Transparent;
			}
		}
	}
}

void GameBoardRenderer::clearBrushVerts()
{
	m_brush_verts = sf::VertexArray(sf::Quads, m_brush->height * m_brush->width * 4);
}

bool GameBoardRenderer::checkUpdateTime(sf::Time elapsed)
{
	m_time_since_update += elapsed;
	if (m_speed == GameSpeed::SPEED_SLOW &&
		m_time_since_update >= SLOW_TIMEOUT)
	{
		m_time_since_update = sf::Time::Zero;
		return true;
	}
	else if (m_speed == GameSpeed::SPEED_FAST &&
		m_time_since_update >= FAST_TIMEOUT)
	{
		m_time_since_update = sf::Time::Zero;
		return true;
	}
	return false;
}

void GameBoardRenderer::update(sf::Time elapsed)
{
	if (m_speed != GameSpeed::SPEED_PAUSED && checkUpdateTime(elapsed))
	{
		m_board.update();
	}
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
	updateBrush();
}

void GameBoardRenderer::stampBrush()
{
	int brush_w = m_brush->width;
	int brush_h = m_brush->height;

	sf::Vector2i brush_coords = sf::Vector2i(m_brush_pos.x / m_cellsize.x, m_brush_pos.y / m_cellsize.y);
	for (int x = 0; x < brush_w; ++x)
	{
		for (int y = 0; y < brush_h; ++y)
		{
			if (m_brush->brush[y * brush_w + x] == 1)
			{
				m_board.setCell(brush_coords + sf::Vector2i(x, y), GameBoard::CELL_ALIVE);
			}
		}
	}
}

void GameBoardRenderer::setBrushPosition(const sf::Vector2i& pos)
{
	// Snap the position to a multiple of cellsize before storing it.
	int tile_x = pos.x / m_cellsize.x;
	int tile_y = pos.y / m_cellsize.y;

	m_brush_pos = sf::Vector2i(tile_x * m_cellsize.x, tile_y * m_cellsize.y);
}