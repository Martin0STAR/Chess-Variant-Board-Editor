#include "Line.h"

using namespace std;

Line::Line()
	:Line::Line{ sf::Vector2f{0.f,0.f}, sf::Vector2f{0.f,0.f}, sf::Color::Red}
{}

Line::Line(sf::Vector2f frompos, sf::Vector2f topos, sf::Color color)
	: _thickness{ 5.f },	_line{ sf::Vector2f{1.f, 1.f} }
{
	_line.setFillColor(color);
	_line.setOrigin(0, _thickness / 2);
	setPosition(frompos, topos);
}

bool Line::operator ==(const Line & rhs) const
{
	return
		_line.getPosition() == rhs._line.getPosition() &&
		_line.getSize() == rhs._line.getSize() &&
		_line.getRotation() == rhs._line.getRotation() &&
		_line.getFillColor() == rhs._line.getFillColor();
}

std::istream& operator>> (istream& is, Line & line)
{
	sf::Vector2f frompos, topos;
	is >> frompos.x >> frompos.y >> topos.x >> topos.y;
	line.setPosition(frompos, topos);
	uint32_t intcolor;
	is >> intcolor;
	sf::Color color{ intcolor };
	line.setColor(color);
	return is;
}

std::ostream& operator<< (ostream& os, const Line & line)
{
	os << line.getFromCoord().x << " "
		<< line.getFromCoord().y << " "
		<< line.getToCoord().x << " "
		<< line.getToCoord().y << " "
		<< line.getColor().toInteger() << "]\n";
	return os;
}

sf::Vector2f Line::getFromCoord() const
{
	return _line.getPosition();
}

sf::Vector2f Line::getToCoord() const
{
	return _to;
}

sf::Color Line::getColor() const
{
	return _line.getFillColor();
}

void Line::setPosition(sf::Vector2f frompos, sf::Vector2f topos)
{
	_to = topos;
	float x = _to.x - frompos.x;
	float y = _to.y - frompos.y;
	float length = sqrt(x*x + y * y);
	float rotation = atan2(y, x);
	_line.setSize(sf::Vector2f(length, _thickness));
	_line.setRotation(rotation*180.f / 3.14f);
	_line.setPosition(frompos);
}

void Line::setColor(sf::Color color)
{
	_line.setFillColor(color);
}

void Line::move(const sf::Vector2f& offset)
{
	_to += offset;
	_line.move(offset);
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_line);
}