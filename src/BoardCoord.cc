#include "BoardCoord.h"

using namespace std;

BoardComponent::Coord::Coord()
	:BoardComponent::Coord::Coord(sf::Vector2i{0,0})
{}

BoardComponent::Coord::Coord(sf::Vector2i coord)
	: _coord{ coord }
{}

BoardComponent::Coord::Coord(unsigned int numrows, string notation)
	: BoardComponent::Coord::Coord{getCoordFromNotation(numrows, notation)}
{}


BoardComponent::Coord::Coord(
	sf::Vector2f offset,
	sf::Vector2f squaresize,
	sf::Vector2f pixelpos)
	: BoardComponent::Coord::Coord{getCoordFromPixelPosition(offset, squaresize, pixelpos)}
{}

bool BoardComponent::Coord::operator ==(const Coord & rhs) const
{
	return _coord.x == rhs._coord.x && _coord.y == rhs._coord.y;
}

bool BoardComponent::Coord::operator <(const Coord & rhs) const
{
	if (_coord.y == rhs._coord.y)
	{
		return _coord.x < rhs._coord.x;
	}
	return _coord.y > rhs._coord.y;
}

bool BoardComponent::Coord::operator >(const Coord & rhs) const
{
	return rhs < *this;
}

BoardComponent::Coord BoardComponent::Coord::operator +(const Coord& rhs) const
{
	return BoardComponent::Coord{ _coord + rhs._coord };
}

sf::Vector2i BoardComponent::Coord::getVector() const
{
	return _coord;
}

sf::Vector2f BoardComponent::Coord::getPixelPosition(
	sf::Vector2f offset, sf::Vector2f squaresize) const
{
	float x = offset.x + (float)_coord.x * squaresize.x;
	float y = offset.y + (float)_coord.y * squaresize.y;
	return sf::Vector2f{ x, y };
}

int BoardComponent::Coord::getColorId(unsigned int numcolors) const
{
	return (_coord.x + _coord.y) % numcolors;
}

string BoardComponent::Coord::getNotation(unsigned int numrows) const
{
	string notation{};
	notation.append(getNotationX());
	notation.append(getNotationY(numrows));
	return notation;
}

string BoardComponent::Coord::getNotationX() const
{
	string notationx{};
	int i;
	if (_coord.x < 0)
	{
		i = -_coord.x - 1;
	}
	else
	{
		i = _coord.x;
	}
	
	for (; i >= 0; i = i / ('z' - 'a' + 1) - 1)
	{
		notationx.insert(notationx.begin(), 'a' + (i % ('z' - 'a' + 1)));
	}
	if (_coord.x < 0)
	{
		notationx.insert(notationx.begin(), '-');
	}
	return notationx;
}

string BoardComponent::Coord::getNotationY(unsigned int numrows) const
{
	return to_string((int)numrows - _coord.y);
}

sf::Vector2i BoardComponent::Coord::getCoordFromNotation(unsigned int numrows, string notation)
{
	sf::Vector2i coord{ 0,0 };
	bool xnegative{ false };
	stringstream ns{ notation }; //ns = notationstream
	if (ns.peek() == '-')
	{
		xnegative = true;
		ns.ignore(1);
	}
	while (isalpha(ns.peek()))
	{
		char c;
		ns >> c;
		coord.x = coord.x * ('z' - 'a') + c - 'a';
	}
	if (xnegative)
	{
		coord.x = -coord.x - 1;
	}
	int yvalue{ 0 };
	ns >> yvalue;
	coord.y = numrows - yvalue;
	return coord;
}

sf::Vector2i BoardComponent::Coord::getCoordFromPixelPosition(
	sf::Vector2f offset,
	sf::Vector2f squaresize,
	sf::Vector2f pixelpos
)
{
	return sf::Vector2i{
		(int)((pixelpos.x - offset.x) / squaresize.x),
		(int)((pixelpos.y - offset.y) / squaresize.y)
	};
}