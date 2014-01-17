#pragma once

#include <SFML/System.hpp>

struct ToolBrush 
{
	int width;
	int height;
	int rotations;
	sf::Uint8 brush[];
};

namespace ToolBrushes {
	const ToolBrush SINGLE_CELL = {
		1, 1, 
		1,
		{
			1,
		}
	};
	const ToolBrush BLOCK = {
		2, 2, 
		1,
		{
			1, 1,
			1, 1,
		}
	};
	const ToolBrush GLIDER = {
		3, 3,
		4,
		{
			0, 1, 0,
			0, 0, 1,
			1, 1, 1,

			0, 1, 1,
			1, 0, 1,
			0, 0, 1,

			1, 1, 1,
			1, 0, 0,
			0, 1, 0,

			1, 0, 0,
			1, 0, 1,
			1, 1, 0,
		}
	};
	const ToolBrush BLINKER = {
		3, 3,
		2,
		{
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,

			0, 0, 0,
			1, 1, 1,
			0, 0, 0,
		}
	};

	inline const sf::Uint8 * getBrushRotation(const ToolBrush * brush, const int rotation) 
	{
		int rot_offset = (rotation % brush->rotations) * brush->height * brush->width;
		return brush->brush + rot_offset;
	}
}
