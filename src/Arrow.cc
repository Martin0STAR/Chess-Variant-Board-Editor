#include "Arrow.h"

using namespace std;

Arrow::Arrow()
	:Arrow::Arrow{ sf::Vector2f{0.f,0.f}, sf::Vector2f{0.f,0.f}, sf::Color::Red}
{
	_hide = true;
}

Arrow::Arrow(sf::Vector2f frompos, sf::Vector2f topos, sf::Color color)
	: _hide{ false }, _thickness{ 5.f }, _headsize{ 15.f },
	_head{ 50,3 }
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
		_to == rhs._to &&
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
	_line.setPosition(frompos);
	_to = topos;
	updateShape();
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

void Arrow::updateShape()
{
	float x = _to.x - _line.getPosition().x;
	float y = _to.y - _line.getPosition().y;
	float length = sqrt(x*x + y * y);

	const unsigned int precision{ (unsigned int)_thickness * 2 };

	_line.setPointCount(precision + 2);
	size_t index{ 0 };
	_line.setPoint(index, sf::Vector2f{ 0.f,0.f });
	index++;
	_line.setPoint(index, sf::Vector2f{ 1.f + length - _headsize * 3.f / 2.f, 0.f });
	index++;
	_line.setPoint(index, sf::Vector2f{ 1.f + length - _headsize * 3.f / 2.f, _thickness });
	index++;
	_line.setPoint(index, sf::Vector2f{ 0.f, _thickness });
	for (unsigned int i{ 1 }; i < precision - 1; i++)
	{
		float radious = ((float)i / (float)precision) * 3.14f;
		float xpos = (-sin(radious)) * _thickness / 2.f;
		float ypos = _thickness - (1 - cos(radious)) * _thickness / 2.f;
		_line.setPoint(index, sf::Vector2f{ xpos, ypos });
		index++;
	}

	float rotation = atan2(y, x);
	_line.setRotation(rotation*180.f / 3.14f);
	_head.setRotation(90 + rotation * 180.f / 3.14f);
	_head.setPosition(
		_line.getPosition().x + (cos(rotation))* (length - _headsize),
		_line.getPosition().y + (sin(rotation))* (length - _headsize)
	);
}

void Arrow::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!_hide)
	{
		target.draw(_line);
		target.draw(_head);
	}
}