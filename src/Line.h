#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "BoardCoord.h"
class Line : public sf::Drawable
{
public:
	Line();
	Line(sf::Vector2f frompos, sf::Vector2f topos, sf::Color color);
	
	bool operator ==(const Line &) const;
	friend std::istream & operator >> (std::istream &is, Line& line);
	friend std::ostream & operator << (std::ostream &os, const Line& line);
	sf::Vector2f getFromCoord() const;
	sf::Vector2f getToCoord() const;
	sf::Color getColor() const;
	void setPosition(sf::Vector2f frompos, sf::Vector2f topos);
	void setColor(sf::Color color);
	void move(const sf::Vector2f &);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2f _to;
	float _thickness;
	sf::RectangleShape _line;
};