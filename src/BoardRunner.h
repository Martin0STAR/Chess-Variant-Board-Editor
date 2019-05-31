#pragma once

#include <random>
#include "Board.h"
#include "BoardNameHandler.h"
#include "ToolWindow.h"
#include "PieceHandler.h"
#include "PieceSelectWindow.h"
#include "Window_Action.h"
#include "EventHandler.h"
#include "KeyboardHandler.h"
#include "Textbox.h"
#include <windows.h> //for opening links

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
	std::string _boardtype;
	std::string _defaultboardname;
	Board _board;
	ToolWindow _toolwindow;
	sf::RenderWindow _mainwindow;
	PieceSelectWindow _pieceselectwindow;
	Window_Action _action;
	EventHandler _eventhandler;
	PieceHandler _piecehandler;
	KeyboardHandler _keyboardhandler;
	BoardComponent::Textbox _textbox;
};