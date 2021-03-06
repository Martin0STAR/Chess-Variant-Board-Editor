#pragma once
#include <string>
#include <cctype>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "PrecisionState.h"
namespace BoardComponent
{
	enum Orientation
	{
		TOPLEFT,
		TOP,
		TOPRIGHT,
		LEFT,
		CENTER,
		RIGHT,
		BOTTOMLEFT,
		BOTTOM,
		BOTTOMRIGHT
	};

	class Coord
	{
	public:
		Coord();
		Coord(sf::Vector2i coord, Orientation orientation = BoardComponent::CENTER);
		Coord(unsigned int numrows, std::string notation);
		Coord(sf::Vector2f offset,
			sf::Vector2f squaresize,
			sf::Vector2f pixelpos,
			unsigned int numcolumns,
			unsigned int numrows,
			bool ishorizontalflipped,
			bool isverticalflipped,
			PrecisionState::PrecisionState = PrecisionState::MIDDLE);

		bool operator ==(const Coord & rhs) const;
		bool operator <(const Coord & rhs) const;
		bool operator >(const Coord & rhs) const;
		Coord operator +(const Coord & rhs) const;

		sf::Vector2i getVector() const;
		sf::Vector2f getPixelPosition(
			sf::Vector2f offset, sf::Vector2f squaresize,
			unsigned int numrows, unsigned int numcolumns,
			bool ishorizontalflipped, bool isverticalflipped) const;
		int getColorId(unsigned int numcolors) const;
		std::string getNotation(unsigned int numrows) const;
		std::string getNotationX() const;
		std::string getNotationY(unsigned int numrows) const;
		Orientation getOrientation() const;
	private:
		void setCoordByNotation(unsigned int numrows, std::string notation);
		void setCoordByPixelPosition(
			sf::Vector2f offset,
			sf::Vector2f squaresize,
			sf::Vector2f pixelpos,
			unsigned int numcolumns,
			unsigned int numrows,
			bool ishorizontalflipped,
			bool isverticalflipped,
			PrecisionState::PrecisionState precision);

		sf::Vector2i _coord;
		bool _isgridright;
		bool _isgridtop;
	};
}