#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "BoardCoord.h"
class Arrow : public sf::Drawable
{
public:
	Arrow();
	Arrow(sf::Vector2f frompos, sf::Vector2f topos, sf::Color color);
	
	bool operator ==(const Arrow & rhs) const;
	friend std::istream & operator >> (std::istream &is, Arrow& arrow);
	friend std::ostream & operator << (std::ostream &os, const Arrow& arrow);
	sf::Vector2f getFromCoord() const;
	sf::Vector2f getToCoord() const;
	sf::Color getColor() const;
	void setPosition(sf::Vector2f frompos, sf::Vector2f topos);
	void setColor(sf::Color color);
	void move(const sf::Vector2f & offset);

private:
	void updateShape();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	bool _hide;
	sf::Vector2f _to;
	float _thickness;
	float _headsize;
	sf::ConvexShape _line;
	sf::CircleShape _head;
};