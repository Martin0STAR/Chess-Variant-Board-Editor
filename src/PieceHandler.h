#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#include <iterator>
#include <algorithm>
#include <SFML/Graphics.hpp>
class PieceHandler
{
public:
	PieceHandler();
	PieceHandler(std::string style);
	bool loadStyle(std::string style);
	sf::Vector2f getScale(std::string style);
	std::string getName(std::string style, unsigned int index);
	unsigned int getNumPieces(std::string style);
	sf::Image getImage(std::string style, std::string name, std::string colorname);
	bool exists(std::string style, std::string name);
private:
	std::string getPieceListFileName(std::string style) const;
	std::string getPieceImageFileName(std::string style, std::string name, std::string piececolor) const;

	sf::Vector2i _squaresize;

	struct PieceContainer
	{
		std::string name;
		sf::Image whiteimage;
		sf::Image blackimage;
	};

	struct StyleContainer
	{
		sf::Vector2i size;
		std::vector<PieceContainer> pieces;
	};

	std::map<std::string, StyleContainer> _styles;
};