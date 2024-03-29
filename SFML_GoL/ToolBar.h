#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

enum ToolBarButtonState
{
	OFF = 0,
	ON,
};

enum ToolBarButtons  // Offsets within texture
{
	FILLER = 0,
	PAUSE,
	SLOW,
	FAST,
	ROTATE,
	BRUSH_SINGLE,
	BRUSH_BLOCK,
	BRUSH_GLIDER,
	BRUSH_BLINKER,
	MAX_BUTTON
};

class ToolBar : public sf::Drawable, public sf::Transformable
{
private:
	static const int TOOLBAR_HEIGHT = 32; //px
	static const int TOOLBAR_WIDTH = 800; //px
	static const int NUM_BUTTONS = 25;

	ToolBarButtons m_buttons[NUM_BUTTONS];
	ToolBarButtonState m_button_state[NUM_BUTTONS];
	std::function<void()> m_button_functions[NUM_BUTTONS];
	int m_highlighted;  // Index of currently highlighted button.

	sf::VertexArray m_verts;
	const sf::Texture& m_tex;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	ToolBar(const sf::Texture& texture) :
		m_highlighted(-1),
		m_verts(sf::Quads, 25 * 4),
		m_tex(texture)
	{
		for (int i = 0; i < 25; i++) 
		{
			m_buttons[i] = ToolBarButtons::FILLER;
			m_button_state[i] = ToolBarButtonState::OFF;
			// We need *some* default function for unused buttons, or we'll segfault when the user clicks on them:
			m_button_functions[i] = []() {};  
		}

		m_buttons[1] = ToolBarButtons::PAUSE;
		m_buttons[2] = ToolBarButtons::SLOW;
		m_buttons[3] = ToolBarButtons::FAST;

		m_buttons[5] = ToolBarButtons::ROTATE;

		m_buttons[7] = ToolBarButtons::BRUSH_SINGLE;
		m_buttons[8] = ToolBarButtons::BRUSH_BLOCK;
		m_buttons[9] = ToolBarButtons::BRUSH_GLIDER;
		m_buttons[10] = ToolBarButtons::BRUSH_BLINKER;

		m_button_state[1] = ToolBarButtonState::ON;
		m_button_state[7] = ToolBarButtonState::ON;
	}

	~ToolBar()
	{
	}

	// Update the toolbar (graphically).
	void update();
	// Alert toolbar to a click at given position, relative to upper-left of toolbar.
	void click(const sf::Vector2i& pos);
	// Alert toolbar to a mouseover at given position, relative to upper-left of toolbar.
	void mouseover(const sf::Vector2i& pos);
	// Bind a callback function for given button index.
	void bindFunction(const int button, const std::function<void()> func) { m_button_functions[button] = func; }
	// Clear current mouseover selection.
	void clearSelection() { m_highlighted = -1; }
};

