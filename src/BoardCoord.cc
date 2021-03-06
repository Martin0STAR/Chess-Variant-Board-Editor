#include "BoardCoord.h"

using namespace std;

BoardComponent::Coord::Coord()
	:BoardComponent::Coord::Coord(sf::Vector2i{0,0})
{}

BoardComponent::Coord::Coord(sf::Vector2i coord, BoardComponent::Orientation orientation)
	: _coord{ coord }, _isgridright{ false }, _isgridtop{ false }
{
	switch (orientation)
	{
	case TOPLEFT:
		_coord.x--;
		_isgridright = true;
		_isgridtop = true;
		break;
	case TOP:
		_isgridtop = true;
		break;
	case TOPRIGHT:
		_isgridright = true;
		_isgridtop = true;
		break;
	case LEFT:
		_coord.x--;
		_isgridright = true;
		break;
	case RIGHT:
		_isgridright = true;
		break;
	case BOTTOMLEFT:
		_coord.x--;
		_coord.y++;
		_isgridright = true;
		_isgridtop = true;
		break;
	case BOTTOM:
		_coord.y++;
		_isgridtop = true;
		break;
	case BOTTOMRIGHT:
		_coord.y++;
		_isgridright = true;
		_isgridtop = true;
		break;
	default:
		break;
	}
}

BoardComponent::Coord::Coord(unsigned int numrows, string notation)
	: BoardComponent::Coord::Coord{}
{
	setCoordByNotation(numrows, notation);
}

BoardComponent::Coord::Coord(
	sf::Vector2f offset,
	sf::Vector2f squaresize,
	sf::Vector2f pixelpos,
	unsigned int numcolumns,
	unsigned int numrows,
	bool ishorizontalflipped,
	bool isverticalflipped,
	PrecisionState::PrecisionState precision)
	: BoardComponent::Coord::Coord{}
{
	setCoordByPixelPosition(
		offset,
		squaresize,
		pixelpos,
		numcolumns,
		numrows,
		ishorizontalflipped,
		isverticalflipped,
		precision);
}

bool BoardComponent::Coord::operator ==(const Coord & rhs) const
{
	return _coord.x == rhs._coord.x &&
		_coord.y == rhs._coord.y &&
		_isgridright == rhs._isgridright &&
		_isgridtop == rhs._isgridtop;
}

bool BoardComponent::Coord::operator <(const Coord & rhs) const
{
	if (_coord.y == rhs._coord.y)
	{
		if (_isgridtop == rhs._isgridtop)
		{
			if (_coord.x == rhs._coord.x)
			{
				if (_isgridright == rhs._isgridright)
				{
					return false;
				}
				else
				{
					return _isgridright == false;
				}
			}
			else
			{
				return _coord.x < rhs._coord.x;
			}
		}
		else
		{
			return _isgridtop == true;
		}
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
	sf::Vector2f offset, sf::Vector2f squaresize,
	unsigned int numrows, unsigned int numcolumns,
	bool ishorizontalflipped, bool isverticalflipped) const
{
	float x = offset.x;
	float y = offset.y;

	if (ishorizontalflipped)
	{
		x += (float)(numcolumns - _coord.x - 1) * squaresize.x;
		if (_isgridright)
		{
			x -= squaresize.x / 2.f;
		}
	}
	else
	{
		x += (float)_coord.x * squaresize.x;
		if (_isgridright)
		{
			x += squaresize.x / 2.f;
		}
	}
	if (isverticalflipped)
	{
		y += (float)(numrows - _coord.y - 1) * squaresize.y;
		if (_isgridtop)
		{
			y += squaresize.y / 2.f;
		}
	}
	else
	{
		y += (float)_coord.y * squaresize.y;
		if (_isgridtop)
		{
			y -= squaresize.y / 2.f;
		}
	}
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
	if (_isgridright)
	{
		notationx.push_back('\'');
	}
	return notationx;
}

string BoardComponent::Coord::getNotationY(unsigned int numrows) const
{
	string notationy;
	notationy = to_string((int)numrows - _coord.y);
	if (_isgridtop)
	{
		notationy.push_back('\'');
	}
	return notationy;
}

BoardComponent::Orientation BoardComponent::Coord::getOrientation() const
{
	if (_isgridright && _isgridtop)
	{
		return BoardComponent::Orientation::TOPRIGHT;
	}
	if (_isgridright)
	{
		return BoardComponent::Orientation::RIGHT;
	}
	if (_isgridtop)
	{
		return BoardComponent::Orientation::TOP;
	}
	else
	{
		return BoardComponent::Orientation::CENTER;
	}
}

void BoardComponent::Coord::setCoordByNotation(unsigned int numrows, string notation)
{
	sf::Vector2i coord{ 0,0 };
	bool xnegative{ false };
	stringstream ns{ notation }; //ns = notationstream
	if (ns.peek() == '-')
	{
		xnegative = true;
		ns.ignore(1);
	}
	bool isfirstalpha{ true };
	while (isalpha(ns.peek()))
	{
		char c;
		ns >> c;
		if (isfirstalpha)
		{
			coord.x = c - 'a';
			isfirstalpha = false;
		}
		else
		{
			coord.x = (coord.x+1) * ('z' - 'a') + c - 'a' + 1;
		}
	}
	if (xnegative)
	{
		coord.x = -coord.x - 1;
	}
	if (ns.peek() == '\'')
	{
		ns.ignore(1);
		_isgridright = true;
	}
	int yvalue{ 0 };
	ns >> yvalue;
	coord.y = numrows - yvalue;
	if (ns.peek() == '\'')
	{
		ns.ignore(1);
		_isgridtop = true;
	}
	_coord = coord;
}

void BoardComponent::Coord::setCoordByPixelPosition(
	sf::Vector2f offset,
	sf::Vector2f squaresize,
	sf::Vector2f pixelpos,
	unsigned int numcolumns,
	unsigned int numrows,
	bool ishorizontalflipped,
	bool isverticalflipped,
	PrecisionState::PrecisionState precision)
{
	int xpos = int((pixelpos.x - offset.x) / squaresize.x);
	int ypos = int((pixelpos.y - offset.y) / squaresize.y);
	_coord = sf::Vector2i{ xpos, ypos };
	_isgridright = false;
	_isgridtop = false;
	
	if (precision == PrecisionState::EDGES_AND_MIDDLE)
	{
		float insidexpos = pixelpos.x - offset.x - xpos * squaresize.x;
		float insideypos = pixelpos.y - offset.y - ypos * squaresize.y;
		if (insidexpos < squaresize.x / 4.f)
		{
			_isgridright = true;
			_coord.x--;
		}
		else if (insidexpos > squaresize.x * 3.f / 4.f)
		{
			_isgridright = true;
		}
		if (insideypos < squaresize.y / 4.f)
		{
			_isgridtop = true;
		}
		else if (insideypos > squaresize.y * 3.f / 4.f)
		{
			_isgridtop = true;
			_coord.y++;
		}
		if (ishorizontalflipped)
		{
			_coord.x = numcolumns - _coord.x - 1;
		}
		if (isverticalflipped)
		{
			_coord.y = numrows - _coord.y - 1;
		}
	}
}