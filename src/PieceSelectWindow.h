#pragma once
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "PieceHandler.h"
#include "Board.h"
#include "Tool.h"
#include "Window_Action.h"
#include "Arrow.h"
#include "KeyboardHandler.h"
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
		MUSKETEER_STYLE,
		REMOVE,
		ADD_LINE,
		ADD_ARROW,
		COLOR_SQUARE,
		ADD_PROFILE_BOX,
		REMOVE_SQUARE,
		ADD_ACCESSORY,
		ADD_CHAR_ACCESSORY,
		SET_COLOR,
		ADD_PIECE
	};
}

class PieceSelectWindow
{
public:
	PieceSelectWindow();
	PieceSelectWindow(PieceHandler& piecehandler, Board& board, Tool& tool);
	void load(PieceHandler&, Board& board, Tool& tool);
	bool isOpen();
	void update(PieceHandler&, Board& board, Tool& tool);
	void open(sf::RenderWindow& window);
	void close();
	void clear();
	void draw();
	void display();
	
	bool pollEvent(sf::Event& event);
	Window_Action handleEvent(sf::Event, Board&, Tool&, Tool&, PieceHandler&, KeyboardHandler&);
	
private:
	void initToolList();
	std::string getTypeFromId(PieceHandler& piecehandler, Tool& tool, unsigned int index) const;
	Piece getPieceFromId(PieceHandler& piecehandler, Tool& tool, unsigned int index) const;
	std::string getToolFileName(Piece_Tool::Piece_Tool tool) const;
	void drawTool(PieceHandler& piecehandler, Tool& tool, unsigned int x, unsigned int y);

	sf::Color getSquareColor(unsigned int x, unsigned int y) const;
	unsigned int getToolIndex(unsigned int x, unsigned int y) const;
	sf::RectangleShape getEmptySquare(unsigned int x, unsigned int y) const;

	unsigned int _numcolumns;
	unsigned int _numrows;

	sf::RenderTexture _rendertexture;
	sf::RenderWindow _window;
	sf::Vector2u _displaysquaresize;
	sf::Color _backgroundcolor;

	std::map<Piece_Tool::Piece_Tool, std::string> _toolnames
	{
		{Piece_Tool::SELECT, "select_object"},
		{Piece_Tool::FLIP_HORIZONTAL, "flip_horizontal"},
		{Piece_Tool::FLIP_VERTICAL, "flip_vertical"},
		{Piece_Tool::REMOVE, "remove"},
		{Piece_Tool::ADD_ARROW, "arrow"}
	};

	std::array<std::string, 3> _accessorynames
	{
		"star6point",
		"shield",
		"sword"
	};

	struct Container
	{
		Piece_Tool::Piece_Tool tool;
		unsigned int id;
	};

	std::vector<Container> _toollist;
};