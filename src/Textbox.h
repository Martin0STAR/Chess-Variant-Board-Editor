#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace BoardComponent
{
	class Textbox
	{
	public:
		Textbox();
		Textbox(sf::Vector2f position);
		void setPosition(sf::Vector2f position);
		void setText(std::string text);
		void showText();
		void hideText();
		void draw(sf::RenderTarget & target);
	private:
		std::string getFontFileName();

		bool _show;
		sf::Font _font;
		sf::Text _text;
		unsigned int _textsize;
		sf::Color _displaytextcolor;
		sf::RectangleShape _textbackground;
		sf::Color _textbackgroundcolor;
		unsigned int _textbackgroundleftoffset;
		unsigned int _textbackgroundtopoffset;
	};
}