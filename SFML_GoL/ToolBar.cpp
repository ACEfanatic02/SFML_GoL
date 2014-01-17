#include "ToolBar.h"

void ToolBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = &m_tex;
	target.draw(m_verts, states);
}

void ToolBar::update()
{
	for (int i = 0; i < NUM_BUTTONS; ++i)
	{
		sf::Vertex * quad = &m_verts[i * 4];
		quad[0].position = sf::Vector2f(i * 32, 0);
		quad[1].position = sf::Vector2f((i + 1) * 32, 0);
		quad[2].position = sf::Vector2f((i + 1) * 32, 32);
		quad[3].position = sf::Vector2f(i * 32, 32);

		int voffset = m_button_state[i] * 32;
		if (m_selected == i) {
			voffset = 32 * 2; 
		}
		int type = m_buttons[i];

		quad[0].texCoords = sf::Vector2f(type * 32, voffset);
		quad[1].texCoords = sf::Vector2f((type + 1) * 32, voffset);
		quad[2].texCoords = sf::Vector2f((type + 1) * 32, voffset + 32);
		quad[3].texCoords = sf::Vector2f(type * 32, voffset + 32);
	}
}

void ToolBar::click(const sf::Vector2i& pos)
{
	int button = pos.x / 32;
	m_button_functions[button]();
}

void ToolBar::mouseover(const sf::Vector2i& pos)
{
	m_selected = pos.x / 32;
}