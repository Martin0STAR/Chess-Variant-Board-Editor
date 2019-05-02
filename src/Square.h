#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

namespace BoardComponent
{
	class Square
	{
	public:
		Square();
		Square(sf::Color color, sf::Vector2u size,
			unsigned int cornersize, bool istopleft, bool istopright,
			bool isbottomleft, bool isbottomright);
		void draw(sf::RenderTarget & target, const sf::Vector2f & position);
	private:
		sf::Color _color;
		sf::Vector2u _size;
		unsigned int _cornersize;
		bool _istopleft;
		bool _istopright;
		bool _isbottomleft;
		bool _isbottomright;
	};
}