#include "Textbox.h"

using namespace std;

BoardComponent::Textbox::Textbox()
	:BoardComponent::Textbox::Textbox{sf::Vector2f{0.f,0.f}}
{}

BoardComponent::Textbox::Textbox(sf::Vector2f position)
	: _textsize{ 32 },
	_displaytextcolor{ sf::Color::Black },
	_textbackgroundcolor{ sf::Color::White },
	_textbackgroundleftoffset{ 5 },
	_textbackgroundtopoffset{ 5 }
{
	string filename = getFontFileName();
	if (_font.loadFromFile(filename))
	{
		_text.setFont(_font);
		_text.setCharacterSize(_textsize);
		_text.setFillColor(_displaytextcolor);
		_text.setString("");
		_text.setPosition(position);
	}
	else
	{
		cerr << "unable to find font in " << filename << endl;
	}
}

void BoardComponent::Textbox::setPosition(sf::Vector2f position)
{
	_text.setPosition(position);
}

void BoardComponent::Textbox::setText(string text)
{
	_text.setString(text);

	sf::FloatRect textrect = _text.getLocalBounds();
	_text.setOrigin(textrect.left + textrect.width / 2.f,
		textrect.top + textrect.height / 2.f);

	_textbackground.setSize(sf::Vector2f{
		textrect.width + 2 * _textbackgroundleftoffset,
		textrect.height + 2 * _textbackgroundtopoffset });
	_textbackground.setOrigin(
		_textbackground.getSize().x / 2.f,
		_textbackground.getSize().y / 2.f);
	_textbackground.setPosition(_text.getPosition());
}

void BoardComponent::Textbox::showText()
{
	_show = true;
}

void BoardComponent::Textbox::hideText()
{
	_show = false;
}

void BoardComponent::Textbox::draw(sf::RenderTarget & target)
{
	if (_show)
	{
		target.draw(_textbackground);
		target.draw(_text);
	}
}

std::string BoardComponent::Textbox::getFontFileName()
{
	return "resources\\fonts\\Raleway-SemiBold.ttf";
}