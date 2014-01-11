#pragma once

#include <SFML/System.hpp>

struct ToolBrush 
{
	int width;
	int height;
	sf::Uint8 brush[];
};

namespace ToolBrushes {
	const ToolBrush SINGLE_CELL = {
		1, 1,
		{
			1,
		}
	};
	const ToolBrush BLOCK = {
		2, 2,
		{
			1, 1,
			1, 1,
		}
	};
	const ToolBrush GLIDER = {
		3, 3,
		{
			0, 1, 0,
			0, 0, 1,
			1, 1, 1,
		}
	};
}
