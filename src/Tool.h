#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "Piece.h"
#include "Arrow.h"
#include "Colortool.h"
#include "PieceColor.h"
#include "ProfileBox.h"

namespace Tool_State
{
	enum Tool_State
	{
		SELECT,
		SET_SQUARE_COLOR,
		ADD_PIECE,
		ADD_ARROW,
		ADD_PROFILE_BOX,
		ADD_PIECE_ACCESSORY,
		ADD_PIECE_CHAR_ACCESSORY,
		REMOVE,
		REMOVE_SQUARE,
		PIECE_PICKER
	};
}

class Tool : public sf::Drawable
{
public:
	Tool();
	Tool(sf::Vector2u size,
		Tool_State::Tool_State state, sf::Color squarecolor,
		sf::Color boardcolor);

	Tool(const Tool &);

	Tool& operator =(const Tool& rhs);

	void load(sf::Vector2u size,
		Tool_State::Tool_State state, sf::Color squarecolor,
		sf::Color boardcolor);

	sf::Color getBackgroundColor() const;
	Piece& getPieceBrush();
	unsigned int getNumColors();
	unsigned int getColorIndex();
	sf::Color getSquareColor() const;
	sf::Color getArrowColor() const;
	sf::Color getArrowColor(unsigned int index) const;
	sf::Color getBorderColor() const;
	PieceColor getPieceColor(unsigned int index) const;
	Tool_State::Tool_State getState() const;
	std::string getAccessoryName() const;
	char getCharAccessory() const;
	sf::Color getAccessoryColor() const;
	sf::Color getAccessoryColor(unsigned int index) const;
	std::string getPlayerName() const;
	BoardComponent::ProfileBox getProfileBox() const;

	void setSquareColor(sf::Color color);
	void setState(Tool_State::Tool_State);
	void setColorIndex(unsigned int index);
	void setPosition(sf::Vector2f position);
	void setAccessoryName(std::string name);
	void setCharAccessory(char character);
	void setPlayerName(std::string name);

	void flipPieceHorizontally();
	void flipPieceVertically();
	void invertPieceColor();
	void updateImage();
	
	void drawRemoveSquareTool(
		sf::RenderTarget & target, sf::Vector2f position,
		sf::Vector2u size);

	void drawSetSquareColorTool(
		sf::RenderTarget & target, sf::Vector2f position,
		sf::Vector2u size,
		unsigned int colorindex);

	void drawArrowTool(
		sf::RenderTarget & target, sf::Vector2f position,
		sf::Vector2u size,
		unsigned int colorindex);

	void drawAddProfileBoxTool(
		sf::RenderTarget & target, sf::Vector2f position,
		sf::Vector2u size,
		unsigned int colorindex);

	void drawAccessoryTool(
		sf::RenderTarget & target, sf::Vector2f position,
		sf::Vector2u size,
		unsigned int colorindex, std::string toolname);
private:
	std::string getName();
	std::string getName(Tool_State::Tool_State);
	std::string getFileName();
	std::string getFileName(Tool_State::Tool_State);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Vector2u _size;
	Tool_State::Tool_State _state;
	Piece _piecebrush;
	ColorTool _colortool;
	sf::RenderTexture _rendertexture;
	sf::Color _squarecolor;
	sf::Color _boardcolor;
	sf::Vector2f _position;
	std::string _accessoryname;
	char _characcessory;
	std::string _playername;

	sf::Vector2u _profilesize;
	unsigned int _profileborder1width;
	unsigned int _profileborder2width;
	sf::Color _profilehighlightcolor;

	std::map<Tool_State::Tool_State, std::string> _toolnames
	{
		{Tool_State::SELECT, "select_object"},
		{Tool_State::ADD_ARROW, "arrow"},
		{Tool_State::REMOVE, "remove"},
		{Tool_State::PIECE_PICKER, "piece_picker"}
	};
};