#include "Line.h"

using namespace std;

Line::Line()
	:Line::Line{ sf::Vector2f{0.f,0.f}, sf::Vector2f{0.f,0.f}, sf::Color::Red}
{}

Line::Line(sf::Vector2f frompos, sf::Vector2f topos, sf::Color color)
	: _thickness{ 5.f }
{
	_shape.setFillColor(color);
	_shape.setOrigin(0, _thickness / 2);
	setPosition(frompos, topos);
}

bool Line::operator ==(const Line & rhs) const
{
	return
		(_shape.getPosition() == rhs._shape.getPosition() &&
		_to == rhs._to ||
		_shape.getPosition() == rhs._to &&
		_to == rhs._shape.getPosition()) &&
		_shape.getFillColor() == rhs._shape.getFillColor();
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
	return _shape.getPosition();
}

sf::Vector2f Line::getToCoord() const
{
	return _to;
}

sf::Color Line::getColor() const
{
	return _shape.getFillColor();
}

void Line::setPosition(sf::Vector2f frompos, sf::Vector2f topos)
{
	_shape.setPosition(frompos);
	_to = topos;
	updateShape();
}

void Line::setColor(sf::Color color)
{
	_shape.setFillColor(color);
}

void Line::move(const sf::Vector2f& offset)
{
	_to += offset;
	_shape.move(offset);
}

void Line::updateShape()
{
	float x = _to.x - _shape.getPosition().x;
	float y = _to.y - _shape.getPosition().y;
	float length = sqrt(x*x + y * y);

	const unsigned int precision{ (unsigned int)_thickness*2 };

	_shape.setPointCount(precision * 2);
	size_t index{ 0 };
	_shape.setPoint(index, sf::Vector2f{ 0.f,0.f });
	index++;
	_shape.setPoint(index, sf::Vector2f{ length, 0.f });
	index++;
	for (unsigned int i{ 1 }; i < precision - 1; i++)
	{
		float radious = ((float)i / (float)precision) * 3.14f;
		float xpos = length + sin(radious) * _thickness/2.f;
		float ypos = (1 - cos(radious)) * _thickness/2.f;
		_shape.setPoint(index, sf::Vector2f{ xpos, ypos });
		index++;
	}
	_shape.setPoint(index, sf::Vector2f{ length, _thickness });
	index++;
	_shape.setPoint(index, sf::Vector2f{ 0.f, _thickness });
	for (unsigned int i{ 1 }; i < precision - 1; i++)
	{
		float radious = ((float)i / (float)precision) * 3.14f;
		float xpos = (- sin(radious)) * _thickness / 2.f;
		float ypos = _thickness - (1 - cos(radious)) * _thickness / 2.f;
		_shape.setPoint(index, sf::Vector2f{ xpos, ypos });
		index++;
	}

	float rotation = atan2(y, x);
	_shape.setRotation(rotation*180.f / 3.14f);
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_shape);
}