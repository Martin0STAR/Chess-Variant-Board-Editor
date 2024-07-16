#include "PieceAccessory.h"

using namespace std;

PieceAccessory::PieceAccessory()
	:PieceAccessory::PieceAccessory{"", sf::Color{255,255,255,255}}
{}

PieceAccessory::PieceAccessory(string name, sf::Color color)
	:_name{ name }, _color{ color }, _charactersize{ 28 }, _scale{sf::Vector2f{1.f, 1.f}}
{
	setOnPiecePosition(name);
}

bool PieceAccessory::operator ==(const PieceAccessory & rhs) const
{
	return _name == rhs._name &&
		_color == rhs._color;
}

string PieceAccessory::getName() const
{
	return _name;
}

sf::Color PieceAccessory::getColor() const
{
	return _color;
}

bool PieceAccessory::isCharacter() const
{
	return _name.size() == 1;
}

void PieceAccessory::setPosition(sf::Vector2f pieceposition, sf::Vector2u piecesize)
{
	_pieceposition = pieceposition;
	_piecesize = piecesize;
}

void PieceAccessory::setScale(sf::Vector2f scale)
{
	_scale = scale;
}

string PieceAccessory::getFileName() const
{
	return "resources/pieceaccessories/" + _name + ".png";
}

string PieceAccessory::getFontFileName() const
{
	return "resources\\fonts\\Raleway-SemiBold.ttf";
}

sf::Vector2f PieceAccessory::getPosition(sf::Vector2u size) const
{
	return _pieceposition + getOnPiecePosition(size);
}

sf::Vector2f PieceAccessory::getOnPiecePosition(sf::Vector2u size) const
{
	float x;
	float y;
	switch (_orientationx)
	{
	case OrientationX::LEFT:
		x = _onpiecepositionadder.x;
		break;
	case OrientationX::CENTER:
		x = ((float)_piecesize.x - (float)size.x) / 2.f + _onpiecepositionadder.x;
		break;
	case OrientationX::RIGHT:
		x = (float)_piecesize.x - (float)size.x + _onpiecepositionadder.x;
		break;
	}

	switch (_orientationy)
	{
	case OrientationY::TOP:
		y = _onpiecepositionadder.y;
		break;
	case OrientationY::CENTER:
		y = ((float)_piecesize.y - (float)size.y) / 2.f + _onpiecepositionadder.y;
		break;
	case OrientationY::DOWN:
		y = (float)_piecesize.y - (float)size.y + _onpiecepositionadder.y;
		break;
	}
	return sf::Vector2f{ x, y };
}

void PieceAccessory::setOnPiecePosition(std::string name)
{
	if (name.size() == 1)
	{
		_onpiecepositionadder = sf::Vector2f{ 1.f, 5.f };
	}
	else if (name == "star6point")
	{
		_onpiecepositionadder = sf::Vector2f{ 4.f,1.f };
		_orientationx = OrientationX::RIGHT;
		_orientationy = OrientationY::TOP;
	}
	else if (name == "shield")
	{
		_onpiecepositionadder = sf::Vector2f{ 0.f, 0.f };
		_orientationx = OrientationX::RIGHT;
		_orientationy = OrientationY::DOWN;
	}
	else if (name == "sword")
	{
		_onpiecepositionadder = sf::Vector2f{ 0.f, 0.f };
		_orientationx = OrientationX::LEFT;
		_orientationy = OrientationY::TOP;
	}
}

void PieceAccessory::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isCharacter())
	{
		sf::Font font;
		string filename = getFontFileName();
		if (font.loadFromFile(filename))
		{
			sf::Text text;
			text.setFont(font);
			text.setCharacterSize(_charactersize);
			text.setFillColor(_color);
			text.setString(_name);
			text.setStyle(sf::Text::Bold);
			sf::FloatRect textrect = text.getLocalBounds();
			text.setOrigin(textrect.left + textrect.width / 2.0f,
				textrect.top + textrect.height / 2.0f);

			sf::Vector2f position{
				_pieceposition.x + (float)_piecesize.x / 2.f + _onpiecepositionadder.x,
				_pieceposition.y + (float)_piecesize.y / 2.f + _onpiecepositionadder.y
			};
			text.setPosition(position);
			target.draw(text, states);
		}
		else
		{
			cerr << "Font for character in accessory in file " << filename << " not found." << endl;
		}
	}
	else
	{
		string filename = getFileName();
		if (std::experimental::filesystem::exists(filename))
		{
			sf::Image image;
			image.loadFromFile(getFileName());
			sf::Texture texture;
			texture.loadFromImage(image);
			sf::Sprite sprite{ texture };
			sprite.setPosition(getPosition(image.getSize()));
			sprite.setScale(_scale);
			sprite.setColor(_color);
			target.draw(sprite, states);
		}
		else
		{
			cerr << "Accessory in file " << filename << " not found." << endl;
		}
	}
}