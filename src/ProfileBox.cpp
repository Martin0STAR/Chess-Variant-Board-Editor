#include "ProfileBox.h"

using namespace std;
BoardComponent::ProfileBox::ProfileBox()
{}

BoardComponent::ProfileBox::ProfileBox(
	string profilename, sf::Vector2f position,
	sf::Vector2u size,
	unsigned int border1width, unsigned int border2width,
	sf::Color border1color, sf::Color border2color)
	:_profilename{ profilename }, _position{ position },
	_size{ size }, _border1width{border1width},_border2width{border2width},
	_border1color{ border1color }, _border2color{border2color},
	_ishighlighted{ false }
{
	updateProfilePicture();
}

istream& BoardComponent::operator>> (istream &is, BoardComponent::ProfileBox& profilebox)
{
	unsigned int ishighlighted;
	uint32_t intplayercolor, inthighlightcolor;
	is >> profilebox._profilename >> profilebox._position.x >> profilebox._position.y
		>> profilebox._size.x >> profilebox._size.y
		>> profilebox._border1width >> profilebox._border2width
		>> intplayercolor >> inthighlightcolor >> ishighlighted;
	profilebox._border1color = sf::Color{ intplayercolor };
	profilebox._border2color = sf::Color{ inthighlightcolor };
	if (ishighlighted == 1)
	{
		profilebox.highlight();
	}
	profilebox.updateProfilePicture();
	return is;
}

ostream& BoardComponent::operator<< (ostream& os, const BoardComponent::ProfileBox& profilebox)
{
	if (profilebox.getName() == "")
	{
		os << "noavatar ";
	}
	else
	{
		os << profilebox.getName() << " ";
	}
	os << profilebox.getPosition().x << " "
		<< profilebox.getPosition().y << " "
		<< profilebox.getSize().x << " "
		<< profilebox.getSize().y << " "
		<< profilebox.getBorderWidth() << " "
		<< profilebox.getHighlightWidth() << " "
		<< profilebox.getPlayerColor().toInteger() << " "
		<< profilebox.getHighlightColor().toInteger() << " ";
	if (profilebox.isHighlighted())
	{
		os << "1";
	}
	else
	{
		os << "0";
	}
	return os;
}

string BoardComponent::ProfileBox::getName() const
{
	return _profilename;
}

sf::Vector2f BoardComponent::ProfileBox::getPosition() const
{
	return _position;
}

sf::Vector2f BoardComponent::ProfileBox::getTopLeftPosition() const
{
	return sf::Vector2f{ getLeftX(), getTopY() };
}

sf::Vector2f BoardComponent::ProfileBox::getBottomRightPosition() const
{
	return sf::Vector2f{ getRightX(), getBottomY() };
}

float BoardComponent::ProfileBox::getLeftX() const
{
	return _position.x - (float)_size.x / 2.f;
}

float BoardComponent::ProfileBox::getRightX() const
{
	return _position.x + (float)_size.x / 2.f;
}

float BoardComponent::ProfileBox::getTopY() const
{
	return _position.y - (float)_size.y / 2.f;
}

float BoardComponent::ProfileBox::getBottomY() const
{
	return _position.y + (float)_size.y / 2.f;
}

sf::Vector2u BoardComponent::ProfileBox::getSize() const
{
	return _size;
}

unsigned int BoardComponent::ProfileBox::getBorderWidth() const
{
	return _border1width;
}

unsigned int BoardComponent::ProfileBox::getHighlightWidth() const
{
	return _border2width;
}

sf::Color BoardComponent::ProfileBox::getPlayerColor() const
{
	return _border1color;
}

sf::Color BoardComponent::ProfileBox::getHighlightColor() const
{
	return _border2color;
}

bool BoardComponent::ProfileBox::isHighlighted() const
{
	return _ishighlighted;
}

bool BoardComponent::ProfileBox::isWithinBorder(int x, int y) const
{
	return (float)x >= getLeftX() &&
		(float)x <= getRightX() &&
		(float)y >= getTopY() &&
		(float)y <= getBottomY();
}

bool BoardComponent::ProfileBox::isWithinBorder(float x, float y) const
{
	return x >= getLeftX() &&
		x <= getRightX() &&
		y >= getTopY() &&
		y <= getBottomY();
}

bool BoardComponent::ProfileBox::set(std::string name, sf::Color bordercolor)
{
	_profilename = name;
	_border1color = bordercolor;
	return updateProfilePicture();
}

bool BoardComponent::ProfileBox::setPlayerColor(sf::Color color)
{
	if (_border1color == color)
	{
		return false;
	}
	_border1color = color;
	return true;
}

bool BoardComponent::ProfileBox::setHighLightColor(sf::Color color)
{
	if (_border2color == color)
	{
		return false;
	}
	_border2color = color;
	return true;
}

bool BoardComponent::ProfileBox::setPosition(sf::Vector2f position)
{
	_position = position;
	return true;
}

bool BoardComponent::ProfileBox::unhighlight()
{
	if (!(_ishighlighted))
	{
		return false;
	}
	_ishighlighted = false;
	return true;
}

bool BoardComponent::ProfileBox::highlight()
{
	if (_ishighlighted)
	{
		return false;
	}
	_ishighlighted = true;
	return true;
}

void BoardComponent::ProfileBox::drawProfilePicture(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (_profileimage.getSize().x == 0)
	{
		return;
	}
	sf::Texture texture;
	texture.loadFromImage(_profileimage);
	sf::Sprite sprite;
	sprite.setTexture(texture, true);
	float profilewidth = (float)_size.x - 2.f * (_border1width + _border2width);
	float profileheight = (float)_size.y - 2.f * (_border1width + _border2width);



	sprite.setScale(profilewidth / texture.getSize().x, profileheight / texture.getSize().y);

	sprite.setPosition(_position.x - profilewidth / 2.f, _position.y - profileheight / 2.f);
	
	target.draw(sprite, states);
}

void BoardComponent::ProfileBox::drawBorder(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::RectangleShape rect{ sf::Vector2f{
		(float)(_size.x - 2 * _border2width),
		(float)(_size.y - 2 * _border2width)} };
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(_border1color);
	rect.setOutlineThickness(-(float)_border1width);
	rect.setPosition(getTopLeftPosition() + sf::Vector2f{
		(float)_border2width, (float)_border2width });
	target.draw(rect, states);
}

void BoardComponent::ProfileBox::drawHighlightBorder(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::RectangleShape rect{ sf::Vector2f{
		(float)_size.x,
		(float)_size.y} };
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(_border2color);
	rect.setOutlineThickness(-(float)_border2width);
	rect.setPosition(getTopLeftPosition());
	target.draw(rect, states);
}

void BoardComponent::ProfileBox::move(const sf::Vector2f& offset)
{
	_position += offset;
}

bool BoardComponent::ProfileBox::saveProfileImage()
{
	if (_profileimage.getSize().x == 0 ||
		_profilename == "")
	{
		return false;
	}
	string filename = getImageFileName();
	if (filesystem::exists(filename))
	{
		return false;
	}
	_profileimage.saveToFile(filename);
	return true;
}

bool BoardComponent::ProfileBox::updateProfilePicture()
{
	if (_profilename == "")
	{
		string filename = getNoAvatarFileName();
		_profileimage.loadFromFile(filename);
		return true;
	}

	string filename = getImageFileName();
	if (filesystem::exists(filename))
	{
		_profileimage.loadFromFile(filename);
		return true;
	}
	filename = getNoAvatarFileName();
	_profileimage.loadFromFile(filename);
	return true;
}

string BoardComponent::ProfileBox::getImageFileName()
{
	return "resources\\profiles\\" + _profilename + ".png";
}

string BoardComponent::ProfileBox::getNoAvatarFileName()
{
	return "resources\\profiles\\noavatar.png";
}

void BoardComponent::ProfileBox::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	drawProfilePicture(target, states);
	drawBorder(target, states);
	if (_ishighlighted)
	{
		drawHighlightBorder(target, states);
	}
}