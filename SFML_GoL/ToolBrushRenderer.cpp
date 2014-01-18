#include "ToolBrushRenderer.h"

void ToolBrushRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = m_tex;
	target.draw(m_verts, states);
}

void ToolBrushRenderer::update()
{
	const int brush_w = m_brush->width;
	const int brush_h = m_brush->height;
	const sf::Uint8 * pbrush = ToolBrushes::getBrushRotation(m_brush, m_rotation);

	for (int x = 0; x < brush_w; ++x)
	{
		for (int y = 0; y < brush_h; ++y)
		{
			sf::Vertex * quads = &m_verts[(y * brush_w + x) * 4];
			
			quads[0].position = sf::Vector2f(x * m_cellsize.x, y * m_cellsize.y);
			quads[1].position = sf::Vector2f((x + 1) * m_cellsize.x, y * m_cellsize.y);
			quads[2].position = sf::Vector2f((x + 1) * m_cellsize.x, (y + 1) * m_cellsize.y);
			quads[3].position = sf::Vector2f(x * m_cellsize.x, (y + 1) * m_cellsize.y);

			if (pbrush[y * brush_w + x])
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

sf::Uint8 ToolBrushRenderer::cellAt(int x, int y) const
{
	 int i = y * m_brush->width + x;
	 const sf::Uint8 * pbrush = ToolBrushes::getBrushRotation(m_brush, m_rotation);
	 return pbrush[i];
}
