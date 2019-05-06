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

void BoardComponent::ProfileBox::drawProfilePicture(sf::RenderTarget & target)
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
	
	target.draw(sprite);
}

void BoardComponent::ProfileBox::drawBorder(sf::RenderTarget & target)
{
	sf::RectangleShape rect{ sf::Vector2f{
		(float)(_size.x - 2 * _border2width),
		(float)(_size.y - 2 * _border2width)} };
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(_border1color);
	rect.setOutlineThickness(-(float)_border1width);
	rect.setPosition(getTopLeftPosition() + sf::Vector2f{
		(float)_border2width, (float)_border2width });
	target.draw(rect);
}

void BoardComponent::ProfileBox::drawHighlightBorder(sf::RenderTarget & target)
{
	sf::RectangleShape rect{ sf::Vector2f{
		(float)_size.x,
		(float)_size.y} };
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineColor(_border2color);
	rect.setOutlineThickness(-(float)_border2width);
	rect.setPosition(getTopLeftPosition());
	target.draw(rect);
}

void BoardComponent::ProfileBox::draw(sf::RenderTarget & target)
{
	drawProfilePicture(target);
	drawBorder(target);
	if (_ishighlighted)
	{
		drawHighlightBorder(target);
	}
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
	if (experimental::filesystem::exists(filename))
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
	if (experimental::filesystem::exists(filename))
	{
		_profileimage.loadFromFile(filename);
		return true;
	}
	
	sf::Http http;
	sf::Http::Request request;
	auto it = _urilist.find(_profilename);
	if (it == _urilist.end())
	{
		string filename = getNoAvatarFileName();
		_profileimage.loadFromFile(filename);
		return true;
	}
	
	http.setHost("http://images.chesscomfiles.com");
	request.setUri(_urilist.at(_profilename));
	request.setMethod(sf::Http::Request::Get);
	request.setHttpVersion(1, 1);
	request.setField("from", "me");
	request.setField("Content-Type", "application/x-www-form-urlencoded");
	sf::Http::Response response = http.sendRequest(request);

	if (response.getBody().size() == 0)
	{
		string filename = getNoAvatarFileName();
		_profileimage.loadFromFile(filename);
		return true;
	}
	_profileimage.loadFromMemory(response.getBody().data(), response.getBody().size());
	_profileimage.saveToFile(filename);
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