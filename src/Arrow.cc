#include "Arrow.h"

using namespace std;

Arrow::Arrow()
	:Arrow::Arrow{ sf::Vector2f{0.f,0.f}, sf::Vector2f{0.f,0.f}, sf::Color::Red}
{
	_hide = true;
}

Arrow::Arrow(sf::Vector2f frompos, sf::Vector2f topos, sf::Color color)
	: _hide{ false }, _thickness{ 5.f }, _headsize{ 15.f },
	_line{ sf::Vector2f{1.f, 1.f} }, _head{ 50,3 }
{
	_line.setFillColor(color);
	_line.setOrigin(0, _thickness / 2);
	_head.setFillColor(color);
	_head.setOrigin(_headsize, _headsize);
	_head.setRadius(_headsize);
	setPosition(frompos, topos);
}

bool Arrow::operator ==(const Arrow & rhs) const
{
	return
		_line.getPosition() == rhs._line.getPosition() &&
		_line.getSize() == rhs._line.getSize() &&
		_line.getRotation() == rhs._line.getRotation() &&
		_line.getFillColor() == rhs._line.getFillColor();
}

std::istream& operator>> (istream& is, Arrow & arrow)
{
	sf::Vector2f frompos, topos;
	is >> frompos.x >> frompos.y >> topos.x >> topos.y;
	arrow.setPosition(frompos, topos);
	uint32_t intcolor;
	is >> intcolor;
	sf::Color color{ intcolor };
	arrow.setColor(color);
	return is;
}

std::ostream& operator<< (ostream& os, const Arrow & arrow)
{
	os << arrow.getFromCoord().x << " "
		<< arrow.getFromCoord().y << " "
		<< arrow.getToCoord().x << " "
		<< arrow.getToCoord().y << " "
		<< arrow.getColor().toInteger() << "]\n";
	return os;
}

sf::Vector2f Arrow::getFromCoord() const
{
	return _line.getPosition();
}

sf::Vector2f Arrow::getToCoord() const
{
	return _to;
}

sf::Color Arrow::getColor() const
{
	return _line.getFillColor();
}

void Arrow::setPosition(sf::Vector2f frompos, sf::Vector2f topos)
{
	_to = topos;
	float x = _to.x - frompos.x;
	float y = _to.y - frompos.y;
	float length = sqrt(x*x + y * y);
	float rotation = atan2(y, x);
	_line.setSize(sf::Vector2f(1.f + length - _headsize * 3.f / 2.f, _thickness));
	_line.setRotation(rotation*180.f / 3.14f);
	_line.setPosition(frompos);
	_head.setRotation(90 + rotation * 180.f / 3.14f);
	_head.setPosition(
		frompos.x + (cos(rotation))* (length - _headsize),
		frompos.y + (sin(rotation))* (length - _headsize)
	);
	_hide = false;
}

void Arrow::setColor(sf::Color color)
{
	_line.setFillColor(color);
	_head.setFillColor(color);
}

void Arrow::move(const sf::Vector2f& offset)
{
	_to += offset;
	_line.move(offset);
	_head.move(offset);
}

void Arrow::draw(sf::RenderTarget & target)
{
	if (!_hide)
	{
		target.draw(_line);
		target.draw(_head);
	}
}