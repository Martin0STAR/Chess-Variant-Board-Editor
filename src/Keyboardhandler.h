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
		ENTER_SAVE_AS_BOARDNAME,
		ENTER_GAME_LINK
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
	bool internHandleTextEntered(std::string inputtext, std::string illegalchars);
	bool handleTextWithKey(sf::Keyboard::Key key, std::string illegalchars);
	std::string removeIllegalChars(std::string & text, const std::string illegalchars);

	Keyboard::State _state;
	std::string _text;
	const std::string illegalBoardNameChars = "\\/:?\"<>|\xe\b *.\x10\x16\v\r\n";
	const std::string illegalPlayerNameChars = "\\/:?\"<>|\xe\b*.\x10\x16\v\r\n";
	const std::string illegalLinkChars = "\x10\x16\v\r\n";
	const std::string illegalAccessoryChars = " \x10\x16\v\r\n";
};