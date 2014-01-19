#pragma once
#include <SFML/Graphics.hpp>

#include "ToolBrush.h"

class ToolBrushRenderer : public sf::Drawable, public sf::Transformable
{
private:
	const sf::Texture * m_tex;
	const sf::Vector2u m_cellsize;
	const ToolBrush * m_brush;
	sf::VertexArray m_verts;

	int m_rotation;
	bool m_visible;

	void clearVerts() { m_verts = sf::VertexArray(sf::Quads, 4 * m_brush->width * m_brush->height); } 
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

public:
	ToolBrushRenderer(const sf::Texture * texture, const sf::Vector2u& cellsize) :
		m_tex(texture),
		m_cellsize(cellsize),
		m_brush(&ToolBrushes::SINGLE_CELL),
		m_verts(sf::Quads, 4 * m_brush->width * m_brush->height)
	{
	}

	~ToolBrushRenderer()
	{
	}

	void update();
	void setBrush(const ToolBrush& brush) 
	{ 
		m_brush = &brush;
		m_rotation = 0;
		clearVerts();
	}
	void rotateBrush() 
	{ 
		m_rotation++; 
		clearVerts(); 
	}

	void setVisible(bool visible) 
	{ 
		m_visible = visible;
		clearVerts();
	}

	int getWidth() const { return m_brush->width; }
	int getHeight() const { return m_brush->height; }

	sf::Uint8 cellAt(int x, int y) const;
};

