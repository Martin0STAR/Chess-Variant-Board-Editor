#include "KeyboardHandler.h"

using namespace std;

KeyboardHandler::KeyboardHandler()
	:_state{ Keyboard::State::NORMAL }
{}

void KeyboardHandler::setState(Keyboard::State state)
{
	_state = state;
}

Window_Action KeyboardHandler::handleKeyPress(sf::Keyboard::Key key)
{
	Window_Action action;
	string prevtext{ _text };
	switch (_state)
	{
	case Keyboard::State::ENTER_BOARD_NAME:
		if (handleTextWithKey(key, illegalBoardNameChars))
		{
			action.name = _text;
			_text = "";
			action.state = Window_Action_State::SET_BOARD_NAME;
			return action;
		}
		else if(_text != prevtext)
		{
			action.name = _text;
			action.state = Window_Action_State::DISPLAY_ENTERED_BOARD_NAME;
			return action;
		}
		else
		{
			action.state = Window_Action_State::NOTHING;
			return action;
		}

	case Keyboard::State::ENTER_SAVE_AS_BOARDNAME:
		if (handleTextWithKey(key, illegalBoardNameChars))
		{
			action.name = _text;
			_text = "";
			action.state = Window_Action_State::SAVE_BOARD_AS;
			return action;
		}
		else if (_text != prevtext)
		{
			action.name = _text;
			action.state = Window_Action_State::DISPLAY_SAVE_AS_BOARDNAME;
			return action;
		}
		else
		{
			action.state = Window_Action_State::NOTHING;
			return action;
		}

	case Keyboard::State::ENTER_PROFILE_NAME:
		if (handleTextWithKey(key, illegalPlayerNameChars))
		{
			action.name = _text;
			_text = "";
			action.state = Window_Action_State::SET_PROFILE_IMAGE;
			return action;
		}
		else if (_text != prevtext)
		{
			action.name = _text;
			action.state = Window_Action_State::DISPLAY_ENTERED_PROFILE_NAME;
			return action;
		}
		else
		{
			action.state = Window_Action_State::NOTHING;
			return action;
		}

	case Keyboard::State::ENTER_GAME_LINK:
		if (handleTextWithKey(key, illegalLinkChars))
		{
			action.name = _text;
			_text = "";
			action.state = Window_Action_State::SET_GAME_LINK;
			return action;
		}
		else if (_text != prevtext)
		{
			action.name = _text;
			action.state = Window_Action_State::DISPLAY_ENTERED_GAME_LINK;
			return action;
		}
		else
		{
			action.state = Window_Action_State::NOTHING;
			return action;
		}
	default:
		switch (key)
		{
		case sf::Keyboard::Up:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
				{
					action.state = Window_Action_State::REMOVE_ROW_DOWN;
				}
				else
				{
					action.state = Window_Action_State::ADD_ROW_UP;
				}
				return action;
			}
			break;
		case sf::Keyboard::Down:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
				{
					action.state = Window_Action_State::REMOVE_ROW_UP;
				}
				else
				{
					action.state = Window_Action_State::ADD_ROW_DOWN;
				}
				return action;
			}
			break;
		case sf::Keyboard::Left:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
				{
					action.state = Window_Action_State::REMOVE_COLUMN_RIGHT;
				}
				else
				{
					action.state = Window_Action_State::ADD_COLUMN_LEFT;
				}
				return action;
			}
			break;

		case sf::Keyboard::Right:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
					sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
				{
					action.state = Window_Action_State::REMOVE_COLUMN_LEFT;
				}
				else
				{
					action.state = Window_Action_State::ADD_COLUMN_RIGHT;
				}
				return action;
			}
			break;
		case sf::Keyboard::Escape:
			action.state = Window_Action_State::CLOSE_WINDOW;
			return action;
		case sf::Keyboard::Space:
			action.isLeftTool = true;
			action.toolstate = Tool_State::SELECT;
			action.state = Window_Action_State::SET_TOOL_STATE;
			return action;
		case sf::Keyboard::S:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::SAVE_BOARD;
				return action;
			}
			break;
		case sf::Keyboard::F12:
			action.state = Window_Action_State::ENTER_SAVE_AS_BOARDNAME;
			return action;
		case sf::Keyboard::L:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::LOAD_CURRENT_BOARD;
				return action;
			}
			break;
		case sf::Keyboard::E:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::CLEAR_BOARD;
				return action;
			}
			break;
		case sf::Keyboard::G:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::TOGGLE_GRID;
				return action;
			}
			break;
		case sf::Keyboard::H:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::GENERATE_COLORED_BOARD;
				return action;
			}
			break;
		case sf::Keyboard::N:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::ENTER_BOARD_NAME;
				return action;
			}
			break;
		case sf::Keyboard::P:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::ENTER_PROFILE_NAME;
				return action;
			}
			break;
		case sf::Keyboard::Q:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::FLIP_BOARD_HORIZONTALLY;
				return action;
			}
			break;
		case sf::Keyboard::W:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::FLIP_BOARD_VERTICALLY;
				return action;
			}
			break;
		case sf::Keyboard::K:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::ENTER_GAME_LINK;
				return action;
			}
			break;
		case sf::Keyboard::J:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				action.state = Window_Action_State::DISPLAY_GAME_LINK;
				return action;
			}
			break;
		default:
			action.state = Window_Action_State::NOTHING;
			return action;
		}
		action.state = Window_Action_State::NOTHING;
		return action;
	}
	action.state = Window_Action_State::NOTHING;
	return action;
}

Window_Action KeyboardHandler::handleTextEntered(sf::String inputtext)
{
	Window_Action action;
	switch (_state)
	{
	case Keyboard::State::ENTER_BOARD_NAME:
		internHandleTextEntered(inputtext, illegalBoardNameChars);
		action.name = _text;
		action.state = Window_Action_State::DISPLAY_ENTERED_BOARD_NAME;
		return action;
	case Keyboard::State::ENTER_SAVE_AS_BOARDNAME:
		internHandleTextEntered(inputtext, illegalBoardNameChars);
		action.name = _text;
		action.state = Window_Action_State::DISPLAY_SAVE_AS_BOARDNAME;
		return action;
	case Keyboard::State::ENTER_PROFILE_NAME:
		internHandleTextEntered(inputtext, illegalPlayerNameChars);
		action.name = _text;
		action.state = Window_Action_State::DISPLAY_ENTERED_PROFILE_NAME;
		return action;
	case Keyboard::State::ENTER_GAME_LINK:
		internHandleTextEntered(inputtext, illegalLinkChars);
		action.name = _text;
		action.state = Window_Action_State::DISPLAY_ENTERED_GAME_LINK;
		return action;
	default:
		if (((std::string)inputtext).size() == 1 &&
			illegalAccessoryChars.find(((std::string)inputtext).at(0)) == string::npos)
		{
			action.name = inputtext;
			action.state = Window_Action_State::SET_PIECE_CHAR_ACCESSORY;
			return action;
		}
		action.state = Window_Action_State::NOTHING;
		return action;
	}
}

bool KeyboardHandler::internHandleTextEntered(std::string inputtext, std::string illegalchars)
{
	for (auto it = inputtext.begin(); it < inputtext.end(); it++)
	{
		if (illegalchars.find(*it) == string::npos)
		{
			_text += *it;
		}
	}
	return true;
}

bool KeyboardHandler::handleTextWithKey(sf::Keyboard::Key key, std::string illegalchars)
{
	switch (key)
	{
	case sf::Keyboard::Enter:
		_state = Keyboard::State::NORMAL;
		return true;
	case sf::Keyboard::BackSpace:
		if (_text.size() > 0)
		{
			_text.pop_back();
		}
		return false;
	case sf::Keyboard::V:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
			sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
		{
			string clipboardtext = sf::Clipboard::getString();
			removeIllegalChars(clipboardtext, illegalchars);
			_text += clipboardtext;
		}
		return false;
	default:
		return false;
	}
}

string KeyboardHandler::removeIllegalChars(string & text, const std::string illegalchars)
{
	text.erase(remove_if(text.begin(), text.end(), [illegalchars](char c)
	{
		return illegalchars.find(c) != string::npos;
	}), text.end());
	return text;
}