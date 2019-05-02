#pragma once
#include <string>
#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tool.h"
#include "Window_Action.h"
#include "Arrow.h"
#include "Keyboardhandler.h"
#include "PieceColor.h"
#include "PieceAccessory.h"

namespace Piece_Tool
{
	enum Piece_Tool
	{
		INVALID,
		SELECT,
		FLIP_HORIZONTAL,
		FLIP_VERTICAL,
		INVERT_COLORS,
		REMOVE,
		ADD_ARROW,
		COLOR_SQUARE,
		ADD_PROFILE_BOX,
		REMOVE_SQUARE,
		ACCESSORY_STAR6POINT,
		ACCESSORY_SHIELD,
		ACCESSORY_SWORD,
		ACCESSORY_CHAR
	};
}

class PieceSelectWindow
{
public:
	PieceSelectWindow();
	PieceSelectWindow(Board &, Tool&);
	void load(Board &, Tool &);
	bool isOpen();
	void update(Board &, Tool &);
	void open(sf::RenderWindow &);
	void close();
	void clear();
	void draw();
	void display();
	
	bool pollEvent(sf::Event &);
	Window_Action handleEvent(sf::Event, Board &, Tool &, Tool &, Keyboardhandler &);
	
	private:
	PieceColor getColorFromId(Tool &, int, int) const;
	std::string getTypeFromId(Tool &, int, int) const;
	Piece getPieceFromId(Tool &, int, int) const;
	Piece_Tool::Piece_Tool getPieceToolFromId(unsigned int);
	Piece_Tool::Piece_Tool getPieceToolFromId(int, int);
	std::string getToolName(Piece_Tool::Piece_Tool) const;
	std::string getToolName(int) const;
	std::string getToolName(int, int) const;
	std::string getToolFileName(int) const;
	void drawTool(Tool & tool, unsigned int x, unsigned int y);

	sf::Color getSquareColor(unsigned int x, unsigned int y) const;
	unsigned int getToolIndex(unsigned int x, unsigned int y) const;
	sf::RectangleShape getEmptySquare(unsigned int x, unsigned int y) const;


	unsigned int _numcolumns;
	unsigned int _numrows;
	unsigned int _numtoolrows;

	sf::RenderTexture _rendertexture;
	sf::RenderWindow _window;
	sf::Vector2u _squaresize;
	sf::Color _backgroundcolor;

	std::array<std::string, 34> _pieces
	{
		"pawn",
		"knight",
		"bishop",
		"rook",
		"queen",
		"king",
		"guard",
		"witch",
		"bulldog",
		"dwarf",
		"hunter",
		"timethief",
		"zombie",
		"wirewolf",
		"archbishop",
		"siege_tower",
		"angel",
		"peasant",
		"ball",
		"nazgul",
		"monkey",
		"time_traveller",
		"uniqator",
		"lumberjack",
		"caveman",
		"harpy",
		"xiangqi_cannon",
		"tree",
		"wild_goat",
		"step_rock",
		"step_rook"
	};

	std::map<Piece_Tool::Piece_Tool, std::string> _toolnames
	{
		{Piece_Tool::SELECT, "select_object"},
		{Piece_Tool::FLIP_HORIZONTAL, "flip_horizontal"},
		{Piece_Tool::FLIP_VERTICAL, "flip_vertical"},
		{Piece_Tool::REMOVE, "remove"},
		{Piece_Tool::ADD_ARROW, "arrow"}
	};

	std::array<Piece_Tool::Piece_Tool, 13> _tools
	{
		Piece_Tool::SELECT,
		Piece_Tool::REMOVE_SQUARE,
		Piece_Tool::ADD_PROFILE_BOX,
		Piece_Tool::COLOR_SQUARE,
		Piece_Tool::ADD_ARROW,
		Piece_Tool::REMOVE,
		Piece_Tool::FLIP_HORIZONTAL,
		Piece_Tool::FLIP_VERTICAL,
		Piece_Tool::INVERT_COLORS,
		Piece_Tool::ACCESSORY_STAR6POINT,
		Piece_Tool::ACCESSORY_SHIELD,
		Piece_Tool::ACCESSORY_SWORD,
		Piece_Tool::ACCESSORY_CHAR
	};

};