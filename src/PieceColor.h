#pragma once
#include <string>
#include <SFML/Graphics.hpp>

struct PieceColor
{
	std::string name{ "" };
	sf::Color adder{ sf::Color{0,0,0,0} };
	sf::Color subtracter{ sf::Color{0,0,0,0} };
	sf::Color multiplier{ sf::Color{255,255,255,255} };
};

bool operator==(const PieceColor& lhs, const PieceColor& rhs);
