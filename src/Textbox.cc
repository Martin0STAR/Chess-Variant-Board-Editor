#include "Textbox.h"

using namespace std;

BoardComponent::Textbox::Textbox()
	:BoardComponent::Textbox::Textbox{sf::Vector2f{0.f,0.f}}
{}

BoardComponent::Textbox::Textbox(sf::Vector2f position)
	: _backgroundoffset{5,5}
{
	string filename = getFontFileName();
	if (_font.loadFromFile(filename))
	{
		_text.setFont(_font);
		_text.setCharacterSize(32);
		_text.setFillColor(sf::Color::Black);
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

	_background.setSize(sf::Vector2f{
		textrect.width + 2 * _backgroundoffset.x,
		textrect.height + 2 * _backgroundoffset.y });
	_background.setOrigin(
		_background.getSize().x / 2.f,
		_background.getSize().y / 2.f);
	_background.setPosition(_text.getPosition());
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
		target.draw(_background);
		target.draw(_text);
	}
}

std::string BoardComponent::Textbox::getFontFileName()
{
	return "resources\\fonts\\Raleway-SemiBold.ttf";
}