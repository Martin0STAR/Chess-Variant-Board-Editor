#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "ToolWindow.h"
#include "Window_Action.h"
#include "BoardCoord.h"
#include "KeyboardHandler.h"

class EventHandler
{
public:
	EventHandler();
	
	Window_Action handleEvent(sf::Event event, Board & board,
		ToolWindow & toolwindow, KeyboardHandler & keyboardhandler);
	void toggleGridSelect();
	
private:
		
	float _scalex;
	float _scaley;

	bool _allowintersections;

	int _dragleftfromx;
	int _dragleftfromy;
	int _dragrightfromx;
	int _dragrightfromy;

	std::array<sf::Color, 10> _colorlist
	{
		sf::Color{255,0,0,128},
		sf::Color{0,0,255,128},
		sf::Color{255,255,0,128},
		sf::Color{160,32,240,128},
		sf::Color{0,0,0,128},
		sf::Color{0,255,0,128},
		sf::Color{255,165,0,128},
		sf::Color{0,255,255,128},
		sf::Color{128,128,128,128},
		sf::Color{255,192,203,128}
	};
	std::array<std::string, 10> _colornamelist
	{
		"white",
		"black",
		"red",
		"blue",
		"yellow",
		"purple",
		"orange",
		"teal",
		"gray",
		"pink"
	};
};