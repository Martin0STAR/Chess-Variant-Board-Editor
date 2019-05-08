#include "Piece.h"

using namespace std;

Piece::Piece()
	:Piece::Piece{ "", "", PieceColor{}, ""}
{}

Piece::Piece(string style, string name, PieceColor color, string flags = "")
	:_style{ style }, _name{ name }, _color{ color },
	_iscarried{ false }, _ismirrored{ false }, _isupsidedown{ false },
	_isinvertedcolors{ false }
{
	for (auto c : flags)
	{
		if (c == 'd')
		{
			_iscarried = true;
		}
		if (c == 'm')
		{
			_ismirrored = true;
		}
		if (c == 'u')
		{
			_isupsidedown = true;
		}
		if (c == 'i')
		{
			_isinvertedcolors = true;
		}
	}
}

Piece::Piece(const Piece & piece)
{
	_style = piece._style;
	_color = piece._color;
	_name = piece._name;
	_iscarried = piece.isCarried();
	_ismirrored = piece.isMirrored();
	_isupsidedown = piece.isUpsideDown();
	_isinvertedcolors = piece._isinvertedcolors;
	_color.adder = piece._color.adder;
	_color.subtracter = piece._color.subtracter;
	_color.multiplier = piece._color.multiplier;
	if (piece.isCarrying())
	{
		addPieceOnTop(piece.getPieceOnTop());
	}
	for (auto a : piece._accessories)
	{
		_accessories.push_back(a);
	}
}

Piece::~Piece()
{
	if (isCarrying())
	{
		delete _pieceontop;
		_pieceontop = nullptr;
	}
	_accessories.clear();
}

Piece& Piece::operator =(const Piece& rhs)
{
	_style = rhs._style;
	_color = rhs._color;
	_name = rhs._name;
	_iscarried = rhs.isCarried();
	_ismirrored = rhs.isMirrored();
	_isupsidedown = rhs.isUpsideDown();
	_isinvertedcolors = rhs._isinvertedcolors;
	if (rhs.isCarrying())
	{
		addPieceOnTop(rhs.getPieceOnTop());
	}
	else if (isCarrying())
	{
		removePieceOnTop();
	}
	return *this;
}

bool Piece::operator ==(const Piece & rhs) const
{
	if (_style == rhs._style &&
		_color == rhs._color &&
		_name == rhs._name &&
		_ismirrored == rhs.isMirrored() &&
		_isupsidedown == rhs.isUpsideDown() &&
		_isinvertedcolors == rhs._isinvertedcolors)
	{
		if (isCarrying() && rhs.isCarrying())
		{
			return getPieceOnTop() == rhs.getPieceOnTop();
		}
		return (!(isCarrying()) && !(rhs.isCarrying()));
	}
	return false;
}

istream & operator >> (istream & is, Piece & piece)
{
	string flags;
	is >> piece._style;
	is >> piece._name;
	is >> piece._color.name;
	
	is >> flags;
	for (auto c : flags)
	{
		if (c == 'd')
		{
			piece._iscarried = true;
		}
		if (c == 'm')
		{
			piece._ismirrored = true;
		}
		if (c == 'u')
		{
			piece._isupsidedown = true;
		}
		if (c == 'i')
		{
			piece._isinvertedcolors = true;
		}
	}
	
	if (flags.find('p') != string::npos)
	{
		uint32_t intcolor;
		is >> intcolor;
		sf::Color adder{ intcolor };
		is >> intcolor;
		sf::Color subtracter{ intcolor };
		is >> intcolor;
		sf::Color multiplier{ intcolor };
		piece.setColorAdder(adder);
		piece.setColorSubtracter(subtracter);
		piece.setColorMultiplier(multiplier);
	}
	if (flags.find('a') != string::npos)
	{
		is >> ws;
		is.ignore(1);
		std::string accessoryname;
		uint32_t intcolor;
		while (is.peek() != ']')
		{
			is >> accessoryname >> intcolor;
			piece.addAccessory(accessoryname, sf::Color{ intcolor });
			is >> ws;
		}
	}
	if (flags.find('c') != string::npos)
	{
		is >> ws;
		is.ignore(1);
		Piece pieceontop;
		is >> pieceontop;
		piece.addPieceOnTop(pieceontop);
	}
	return is;
}

ostream &operator <<(ostream &os, const Piece& piece)
{
	os << piece.getStyle()
		<< " " << piece.getName()
		<< " " << piece.getColor().name
		<< " -" << piece.getFlags();
	if (piece.colorIsModified())
	{
		os << " " << piece.getColor().adder.toInteger()
			<< " " << piece.getColor().subtracter.toInteger()
			<< " " << piece.getColor().multiplier.toInteger();
	}
	if (piece.hasAccessories())
	{
		os << " [";
		auto alist = piece.getAccessorylist();
		for (auto it = alist.begin();
			it != alist.end();
			it++)
		{
			os << it->getName() << " " << it->getColor().toInteger();
			if (it + 1 != alist.end())
			{
				os << " ";
			}
		}
		os << "]";
	}
	if (piece.isCarrying())
	{
		os << " [" << piece.getPieceOnTop() << "]";
	}
	return os;
}

string Piece::getStyle() const
{
	return _style;
}

string Piece::getName() const
{
	return _name;
}

PieceColor Piece::getColor() const
{
	return _color;
}

string Piece::getFlags() const
{
	string flags;
	if (_iscarried)
	{
		flags.push_back('d');
	}
	if (_ismirrored)
	{
		flags.push_back('m');
	}
	if (_isupsidedown)
	{
		flags.push_back('u');
	}
	if (_isinvertedcolors)
	{
		flags.push_back('i');
	}
	if (colorIsModified())
	{
		flags.push_back('p');
	}
	if (hasAccessories())
	{
		flags.push_back('a');
	}
	if (isCarrying())
	{
		flags.push_back('c');
	}
	return flags;
}

Piece Piece::getPieceOnTop() const
{
	return *_pieceontop;
}

std::vector<PieceAccessory> Piece::getAccessorylist() const
{
	return _accessories;
}

bool Piece::exists() const
{
	return std::experimental::filesystem::exists(getFileName());
}

bool Piece::isCarrier() const
{
	return _name.substr(0, 4) == "step" &&
		_iscarried == false;
}

bool Piece::isCarrying() const
{
	return _pieceontop != nullptr;
}

bool Piece::isCarried() const
{
	return _iscarried;
}

bool Piece::isMirrored() const
{
	return _ismirrored;
}

bool Piece::isUpsideDown() const
{
	return _isupsidedown;
}

bool Piece::isInvertedColors() const
{
	return _isinvertedcolors;
}

bool Piece::colorIsModified() const
{
	return _color.adder != sf::Color{ 0,0,0,0 } ||
		_color.subtracter != sf::Color{ 0,0,0,0 } ||
		_color.multiplier != sf::Color{ 255,255,255,255 };
}

bool Piece::hasAccessories() const
{
	return _accessories.size() != 0;
}

void Piece::setStyle(string style)
{
	_style = style;
}

void Piece::setName(string name)
{
	_name = name;
}

void Piece::setColor(PieceColor color)
{
	_color = color;
}

void Piece::setColorAdder(sf::Color color)
{
	_color.adder = color;
}

void Piece::setColorSubtracter(sf::Color color)
{
	_color.subtracter = color;
}

void Piece::setColorMultiplier(sf::Color color)
{
	_color.multiplier = color;
}


void Piece::setWidth(unsigned int width)
{
	_size.x = width;
	if (isCarrying())
	{
		_pieceontop->setWidth(width);
	}
}

void Piece::setSize(sf::Vector2u size)
{
	_size = size;
}

void Piece::setHeight(unsigned int height)
{
	_size.y = height;
	if (isCarrying())
	{
		_pieceontop->setHeight(height);
	}
}

bool Piece::addPieceOnTop(Piece pieceontop)
{
	if (!isCarrier())
	{
		return false;
	}
	if (isCarrying())
	{
		if (getPieceOnTop() == pieceontop)
		{
			removePieceOnTop();
			return true;
		}
		removePieceOnTop();
	}
	Piece* newpieceontop = new Piece{ pieceontop };
	newpieceontop->_iscarried = true;
	_pieceontop = newpieceontop;
	return true;
}

bool Piece::addAccessory(std::string accessoryname, sf::Color color)
{
	auto it = find(
		_accessories.begin(), _accessories.end(),
		PieceAccessory{ accessoryname, color });
	if (it == _accessories.end())
	{
		auto it2 = find_if(_accessories.begin(), _accessories.end(), [accessoryname](PieceAccessory a)
		{
			return a.isCharacter() || a.getName() == accessoryname;
		});
		if (it2 != _accessories.end())
		{
			_accessories.erase(it2);
		}
		_accessories.push_back(PieceAccessory{ accessoryname, color });
	}
	else
	{
		_accessories.erase(it);
	}
	return true;
}

bool Piece::removePieceOnTop()
{
	if (isCarrying())
	{
		delete _pieceontop;
		_pieceontop = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}

bool Piece::flipHorizontally()
{
	_ismirrored = !_ismirrored;
	return true;
}

bool Piece::flipVertically()
{
	_isupsidedown = !_isupsidedown;
	return true;
}

bool Piece::invertColors()
{
	_isinvertedcolors = !_isinvertedcolors;
	return true;
}

void Piece::draw(sf::RenderTarget & target, const sf::Vector2f & position)
{
	if (isCarrying())
	{
		_pieceontop->draw(target, position);
	}

	string filename = getFileName();
	if (std::experimental::filesystem::exists(filename))
	{
		sf::Image pieceimage;
		pieceimage.loadFromFile(filename);
		if (isInvertedColors())
		{
			invertColor(pieceimage);
		}
		offsetColor(pieceimage);
		if (isMirrored())
		{
			pieceimage.flipHorizontally();
		}
		if (isUpsideDown())
		{
			pieceimage.flipVertically();
		}

		sf::Texture piecetexture;
		piecetexture.loadFromImage(pieceimage);
		piecetexture.setSmooth(false);
		sf::Sprite piecesprite{ piecetexture };

		if (isCarrier())
		{
			piecesprite.setPosition(
				position + sf::Vector2f{
					((float)_size.x - pieceimage.getSize().x) / 2.f,
					(float)_size.y / 2.f
				}
			);
		}
		else if (isCarried())
		{
			float piecewidth = (float)pieceimage.getSize().x;
			float pieceheight = (float)pieceimage.getSize().y;
			if ((float)pieceimage.getSize().y > (float)_size.y / 2.f)
			{
				piecesprite.setScale(0.5f, 0.5f);
				piecewidth = piecewidth / 2.f;
				pieceheight = pieceheight / 2.f;
			}
			piecesprite.setPosition(
				position + sf::Vector2f{
					((float)_size.x - piecewidth) / 2,
					(float)_size.y / 2.f - pieceheight
				}
			);
		}
		else
		{
			int xpos = (_size.x - pieceimage.getSize().x) / 2;
			int ypos = (_size.y - pieceimage.getSize().y) / 2;
			piecesprite.setPosition(
				position + sf::Vector2f{(float)xpos, (float)ypos});
		}
		piecesprite.setColor(_color.multiplier);
		target.draw(piecesprite);
		for (auto &a : _accessories)
		{
			a.setPosition(piecesprite.getPosition(), pieceimage.getSize());
			a.setScale(piecesprite.getScale());
			target.draw(a);
		}
	}
}

string Piece::getFileName() const
{
	return "resources/pieces/" + _style + "/" + _color.name + "/" + _name + ".png";
}

void Piece::invertColor(sf::Image & image) const
{
	for (unsigned int x{ 0 }; x < image.getSize().x; x++)
	{
		for (unsigned int y{ 0 }; y < image.getSize().y; y++)
		{
			sf::Color oldcolor = image.getPixel(x, y);
			sf::Color newcolor;
			newcolor.r = 255 - oldcolor.r;
			newcolor.g = 255 - oldcolor.g;
			newcolor.b = 255 - oldcolor.b;
			newcolor.a = oldcolor.a;
			image.setPixel(x,y,newcolor);
		}
	}
}

void Piece::offsetColor(sf::Image & image) const
{
	if (_color.adder == sf::Color{ 0,0,0,0 } &&
		_color.subtracter == sf::Color{ 0,0,0,0 })
	{
		return;
	}
	for (unsigned int x{ 0 }; x < image.getSize().x; x++)
	{
		for (unsigned int y{ 0 }; y < image.getSize().y; y++)
		{
			image.setPixel(
				x,
				y,
				image.getPixel(x, y) + _color.adder - _color.subtracter);
		}
	}
}