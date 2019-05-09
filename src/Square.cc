#include "Square.h"

BoardComponent::Square::Square()
	:BoardComponent::Square::Square{sf::Color::White, sf::Vector2u{0,0},
0,false,false,false,false}
{}

BoardComponent::Square::Square(sf::Color color, sf::Vector2u size,
	unsigned int cornersize, bool istopleft, bool istopright,
	bool isbottomleft, bool isbottomright)
	: _size{ size }, _cornersize{cornersize},
	_istopleft{istopleft}, _istopright{istopright},
	_isbottomleft{ isbottomleft }, _isbottomright{ isbottomright }
{
	_shape.setFillColor(color);
	updateShape();
}

void BoardComponent::Square::setPosition(sf::Vector2f position)
{
	_shape.setPosition(position);
}

bool BoardComponent::Square::updateShape()
{
	if (!_istopleft && !_istopright && !_isbottomleft && !_isbottomright
		|| _cornersize == 0)
	{
		_shape.setPointCount(4);
		_shape.setPoint(0, sf::Vector2f{ 0.f,0.f });
		_shape.setPoint(1, sf::Vector2f{ (float)_size.x, 0.f });
		_shape.setPoint(2, sf::Vector2f{ (float)_size.x, (float)_size.y });
		_shape.setPoint(3, sf::Vector2f{ 0.f, (float)_size.y });
		return true;
	}

	const unsigned int precision{ _cornersize };
	size_t pointcount{ 12 };
	if (_istopleft)
	{
		pointcount += precision - 2;
	}
	if (_istopright)
	{
		pointcount += precision - 2;
	}
	if (_isbottomright)
	{
		pointcount += precision - 2;
	}
	if (_isbottomleft)
	{
		pointcount += precision - 2;
	}
	size_t index{ 0 };
	_shape.setPointCount(pointcount);
	_shape.setPoint(index, sf::Vector2f(0.f, (float)_cornersize));
	index++;
	if (_istopleft)
	{
		for (unsigned int i{ 1 }; i < precision; i++)
		{
			float radious = ((float)i / (float)precision) * 3.14f / 2.f;
			float xpos = (1 - cos(radious)) * (float)_cornersize;
			float ypos = (1 - sin(radious)) * (float)_cornersize;
			_shape.setPoint(index, sf::Vector2f{ xpos, ypos });
			index++;
		}
	}
	else
	{
		_shape.setPoint(index, sf::Vector2f{ 0.f, 0.f });
		index++;
	}
	_shape.setPoint(index, sf::Vector2f{ (float)_cornersize, 0.f });
	index++;
	_shape.setPoint(index, sf::Vector2f{ (float)(_size.x - _cornersize), 0.f });
	index++;
	if (_istopright)
	{
		for (unsigned int i{ 1 }; i < precision; i++)
		{
			float radious = ((float)i / (float)precision) * 3.14f / 2.f;
			float xpos = (float)_size.x - (1 - sin(radious)) * (float)_cornersize;
			float ypos = (1 - cos(radious)) * (float)_cornersize;
			_shape.setPoint(index, sf::Vector2f{ xpos, ypos });
			index++;
		}
	}
	else
	{
		_shape.setPoint(index, sf::Vector2f{ (float)_size.x, 0.f });
		index++;
	}
	_shape.setPoint(index, sf::Vector2f{ (float)_size.x, (float)_cornersize });
	index++;
	_shape.setPoint(index, sf::Vector2f{ (float)_size.x, (float)(_size.y - _cornersize) });
	index++;
	if (_isbottomright)
	{
		for (unsigned int i{ 1 }; i < precision; i++)
		{
			float radious = ((float)i / (float)precision) * 3.14f / 2.f;
			float xpos = (float)_size.x - (1 - cos(radious)) * (float)_cornersize;
			float ypos = _size.y - (1 - sin(radious)) * (float)_cornersize;
			_shape.setPoint(index, sf::Vector2f{ xpos, ypos });
			index++;
		}
	}
	else
	{
		_shape.setPoint(index, sf::Vector2f{ (float)_size.x, (float)_size.y });
		index++;
	}
	_shape.setPoint(index, sf::Vector2f{ (float)(_size.x - _cornersize), (float)_size.y });
	index++;
	_shape.setPoint(index, sf::Vector2f{ (float)_cornersize, (float)_size.y });
	index++;

	if (_isbottomleft)
	{
		for (unsigned int i{ 1 }; i < precision; i++)
		{
			float radious = ((float)i / (float)precision) * 3.14f / 2.f;
			float xpos = (1 - sin(radious)) * (float)_cornersize;
			float ypos = _size.y - (1 - cos(radious)) * (float)_cornersize;
			_shape.setPoint(index, sf::Vector2f{ xpos, ypos });
			index++;
		}
	}
	else
	{
		_shape.setPoint(index, sf::Vector2f{ 0.f, (float)_size.y });
		index++;
	}
	_shape.setPoint(index, sf::Vector2f{ 0.f, (float)(_size.y - _cornersize) });
	return true;
}

void BoardComponent::Square::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_shape, states);
}