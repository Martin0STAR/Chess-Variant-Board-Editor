#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "PieceColor.h"
class ColorTool
{
public:
	ColorTool();
	ColorTool(unsigned int index);
	unsigned int getSize();
	unsigned int getIndex();

	PieceColor getPieceColor() const;
	PieceColor getPieceColor(unsigned int index) const;
	sf::Color getSquareColor() const;
	sf::Color getSquareColor(unsigned int index) const;
	sf::Color getArrowColor() const;
	sf::Color getArrowColor(unsigned int index) const;
	sf::Color getBorderColor() const;
	sf::Color getBorderColor(unsigned int index) const;
	sf::Color getAccessoryColor() const;
	sf::Color getAccessoryColor(unsigned int index) const;
	void setIndex(unsigned int index);
private:
	void intern_load();

	struct ColorNode
	{
		PieceColor piececolor;
		sf::Color squarecolor;
		sf::Color arrowcolor;
		sf::Color bordercolor;
		sf::Color accessorycolor;
	};

	unsigned int _index;
	std::vector<ColorNode> _colorlist;
};