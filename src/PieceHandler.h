#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <iterator>
#include <SFML/Graphics.hpp>
#include "Piece.h"
class PieceHandler
{
public:
	PieceHandler();
	PieceHandler(std::string style);
	bool loadStyle(std::string style);
	sf::Vector2f getScale(std::string style);
	std::vector<std::string> getNameList(std::string style);
private:
	std::string getPieceListFileName(std::string style);

	sf::Vector2i _squaresize;
	struct StyleContainer
	{
		sf::Vector2i size;
		std::vector<std::string> piecenames;
	};

	std::map<std::string, StyleContainer> _styles;
};