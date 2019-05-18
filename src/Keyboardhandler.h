#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Window_Action.h"

namespace Keyboard
{
	enum State
	{
		NORMAL,
		ENTER_BOARD_NAME,
		ENTER_PROFILE_NAME,
		ENTER_SAVE_AS_BOARDNAME
	};
}

class KeyboardHandler
{
public:
	KeyboardHandler();

	void setState(Keyboard::State state);

	Window_Action handleKeyPress(sf::Keyboard::Key key);

	Window_Action handleTextEntered(sf::String inputtext);

private:
	std::string removeIllegalBoardNameChars(std::string &);
	std::string removeIllegalPlayerNameChars(std::string &);
	Keyboard::State _state;
	std::string _text;
	const std::string illegalBoardNameChars = "\\/:?\"<>|\xe\b *.\x10\x16";
	const std::string illegalPlayerNameChars = "\\/:?\"<>|\xe\b*.\x10\x16";
};