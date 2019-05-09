#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

namespace BoardComponent
{
	class Square : public sf::Drawable
	{
	public:
		Square();
		Square(sf::Color color, sf::Vector2u size,
			unsigned int cornersize, bool istopleft, bool istopright,
			bool isbottomleft, bool isbottomright);
		void setPosition(sf::Vector2f position);
		bool updateShape();
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		sf::Vector2u _size;
		unsigned int _cornersize;
		bool _istopleft;
		bool _istopright;
		bool _isbottomleft;
		bool _isbottomright;
		sf::ConvexShape _shape;
	};
}