#include "Keyboardhandler.h"

using namespace std;

Keyboardhandler::Keyboardhandler()
	:_state{ Keyboard::State::NORMAL }
{}

void Keyboardhandler::setState(Keyboard::State state)
{
	_state = state;
}

Window_Action Keyboardhandler::handleKeyPress(sf::Keyboard::Key key)
{
	Window_Action action;
	switch (_state)
	{
	case Keyboard::State::ENTER_BOARD_NAME:
		switch (key)
		{
		case sf::Keyboard::Enter:
			action.name = _text;
			_state = Keyboard::State::NORMAL;
			_text = "";
			action.state = Window_Action_State::SET_BOARD_NAME;
			return action;
		case sf::Keyboard::BackSpace:
			if (_text.size() > 0)
			{
				_text.pop_back();
				action.name = _text;
				action.state = Window_Action_State::DISPLAY_ENTERED_BOARD_NAME;
				return action;
			}
			action.state = Window_Action_State::NOTHING;
			return action;
		case sf::Keyboard::V:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				string clipboardtext = sf::Clipboard::getString();
				removeIllegalBoardNameChars(clipboardtext);
				_text += clipboardtext;
				action.name = _text;
				action.state = Window_Action_State::DISPLAY_ENTERED_BOARD_NAME;
				return action;
			}
			action.state = Window_Action_State::NOTHING;
			return action;
		default:
			action.state = Window_Action_State::NOTHING;
			return action;
		}
	case Keyboard::State::ENTER_SAVE_AS_BOARDNAME:
		switch (key)
		{
		case sf::Keyboard::Enter:
			action.name = _text;
			_state = Keyboard::State::NORMAL;
			_text = "";
			action.state = Window_Action_State::SAVE_BOARD_AS;
			return action;
		case sf::Keyboard::BackSpace:
			if (_text.size() > 0)
			{
				_text.pop_back();
				action.name = _text;
				action.state = Window_Action_State::DISPLAY_SAVE_AS_BOARDNAME;
				return action;
			}
			action.state = Window_Action_State::NOTHING;
			return action;
		case sf::Keyboard::V:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				string clipboardtext = sf::Clipboard::getString();
				removeIllegalBoardNameChars(clipboardtext);
				_text += clipboardtext;
				action.name = _text;
				action.state = Window_Action_State::DISPLAY_SAVE_AS_BOARDNAME;
				return action;
			}
			action.state = Window_Action_State::NOTHING;
			return action;
		default:
			action.state = Window_Action_State::NOTHING;
			return action;
		}
	case Keyboard::State::ENTER_PROFILE_NAME:
		switch (key)
		{
		case sf::Keyboard::Enter:
			action.name = _text;
			_state = Keyboard::State::NORMAL;
			_text = "";
			action.state = Window_Action_State::SET_PROFILE_IMAGE;
			return action;
		case sf::Keyboard::BackSpace:
			if (_text.size() > 0)
			{
				_text.pop_back();
				action.name = _text;
				action.state = Window_Action_State::DISPLAY_ENTERED_PROFILE_NAME;
				return action;
			}
			action.state = Window_Action_State::NOTHING;
			return action;
		case sf::Keyboard::V:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			{
				string clipboardtext = sf::Clipboard::getString();
				removeIllegalPlayerNameChars(clipboardtext);
				_text += clipboardtext;
				action.name = _text;
				action.state = Window_Action_State::DISPLAY_ENTERED_PROFILE_NAME;
				return action;
			}
			action.state = Window_Action_State::NOTHING;
			return action;
		}
		action.state = Window_Action_State::NOTHING;
		return action;
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

Window_Action Keyboardhandler::handleTextEntered(sf::String inputtext)
{
	Window_Action action;
	switch (_state)
	{
	case Keyboard::State::ENTER_BOARD_NAME:
		for (auto it = inputtext.begin(); it < inputtext.end(); it++)
		{
			if (illegalBoardNameChars.find(*it) == string::npos)
			{
				_text += *it;
			}
		}
		action.name = _text;
		action.state = Window_Action_State::DISPLAY_ENTERED_BOARD_NAME;
		return action;
	case Keyboard::State::ENTER_SAVE_AS_BOARDNAME:
		for (auto it = inputtext.begin(); it < inputtext.end(); it++)
		{
			if (illegalBoardNameChars.find(*it) == string::npos)
			{
				_text += *it;
			}
		}
		action.name = _text;
		action.state = Window_Action_State::DISPLAY_SAVE_AS_BOARDNAME;
		return action;
	case Keyboard::State::ENTER_PROFILE_NAME:
		for (auto it = inputtext.begin(); it < inputtext.end(); it++)
		{
			if (illegalPlayerNameChars.find(*it) == string::npos)
			{
				_text += *it;
			}
		}
		action.name = _text;
		action.state = Window_Action_State::DISPLAY_ENTERED_PROFILE_NAME;
		return action;
	default:
		action.name = inputtext;
		if (action.name.size() == 1 &&
			action.name != " ")
		{
			action.state = Window_Action_State::SET_PIECE_CHAR_ACCESSORY;
			return action;
		}
		action.state = Window_Action_State::NOTHING;
		return action;
	}
}

string Keyboardhandler::removeIllegalBoardNameChars(std::string & text)
{
	text.erase(remove_if(text.begin(), text.end(), [this](char c)
	{
		return illegalBoardNameChars.find(c) != string::npos;
	}), text.end());
	return text;
}

string Keyboardhandler::removeIllegalPlayerNameChars(std::string & text)
{
	text.erase(remove_if(text.begin(), text.end(), [this](char c)
	{
		return illegalPlayerNameChars.find(c) != string::npos;
	}), text.end());
	return text;
}