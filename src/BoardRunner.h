#pragma once

#include <random>
#include "Board.h"
#include "BoardNameHandler.h"
#include "ToolWindow.h"
#include "PieceHandler.h"
#include "PieceSelectWindow.h"
#include "Window_Action.h"
#include "Event_handler.h"
#include "Keyboardhandler.h"
#include "Textbox.h"

class BoardRunner
{
public:
	BoardRunner(std::string);
	~BoardRunner();
	void run(std::default_random_engine & rng);
private:
	BoardNameHandler _boardnamelist;
	unsigned int _boardnumber;
	unsigned int _numboards;
	std::string _boardname;
	Board _board;
	ToolWindow _toolwindow;
	sf::RenderWindow _mainwindow;
	PieceSelectWindow _pieceselectwindow;
	Window_Action _action;
	Event_handler _event_handler;
	PieceHandler _piecehandler;
	Keyboardhandler _keyboardhandler;
	BoardComponent::Textbox _textbox;
};