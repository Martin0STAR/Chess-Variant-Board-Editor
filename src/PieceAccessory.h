#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <filesystem>
#include <iostream>


namespace OrientationX
{
	enum OrientationX
	{
		LEFT,
		CENTER,
		RIGHT
	};
}

namespace OrientationY
{
	enum OrientationY
	{
		TOP,
		CENTER,
		DOWN
	};
}

class PieceAccessory : public sf::Drawable
{
public:
	PieceAccessory();
	PieceAccessory(std::string name, sf::Color color);

	bool operator ==(const PieceAccessory &) const;

	std::string getName() const;
	sf::Color getColor() const;

	bool isCharacter() const;

	void setPosition(sf::Vector2f pieceposition, sf::Vector2u piecesize);
	void setScale(sf::Vector2f scale);
	
private:
	std::string getFileName() const;
	std::string getFontFileName() const;
	sf::Vector2f getPosition(sf::Vector2u size) const;
	sf::Vector2f getOnPiecePosition(sf::Vector2u size) const;

	void setOnPiecePosition(std::string name);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::string _name;
	sf::Color _color;
	sf::Vector2f _pieceposition;
	unsigned int _charactersize;
	sf::Vector2u _piecesize;
	sf::Vector2f _onpiecepositionadder;
	sf::Vector2f _scale;
	OrientationX::OrientationX _orientationx;
	OrientationY::OrientationY _orientationy;
	
};