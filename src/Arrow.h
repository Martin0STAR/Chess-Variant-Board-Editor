#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "BoardCoord.h"
class Arrow
{
public:
	Arrow();
	Arrow(sf::Vector2f frompos, sf::Vector2f topos, sf::Color color);
	
	bool operator ==(const Arrow &) const;
	sf::Vector2f getFromCoord() const;
	sf::Vector2f getToCoord() const;
	sf::Color getColor() const;
	void setPosition(sf::Vector2f frompos, sf::Vector2f topos);
	void move(const sf::Vector2f &);
	void draw(sf::RenderTarget &);

private:
	bool _hide;
	sf::Vector2f _to;
	float _thickness;
	float _headsize;
	sf::RectangleShape _line;
	sf::CircleShape _head;
};