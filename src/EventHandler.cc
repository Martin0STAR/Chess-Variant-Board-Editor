#include "EventHandler.h"

using namespace std;

EventHandler::EventHandler()
	:_scalex{ 1.0 }, _scaley{ 1.0 }, _allowintersections{ false }
{}

Window_Action EventHandler::handleEvent(sf::Event event, Board & board,
	ToolWindow & toolwindow, Keyboardhandler & keyboardhandler)
{
	Window_Action action;
	switch(event.type)
	{
		case sf::Event::Closed:
			action.state = Window_Action_State::CLOSE_WINDOW;
			return action;
		case sf::Event::Resized:
			_scalex = (float)board.getDisplaySize().x / (float)event.size.width;
			_scaley = (float)(board.getDisplaySize().y + (float)toolwindow.getSize().y) / (float)event.size.height;
			break;
		case sf::Event::KeyPressed:
			return keyboardhandler.handleKeyPress(event.key.code);
		case sf::Event::TextEntered:
			return keyboardhandler.handleTextEntered(event.text.unicode);
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				action.isLeftTool = true;
				_dragleftfromx = event.mouseButton.x;
				_dragleftfromy = event.mouseButton.y;
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				action.isLeftTool = false;
				_dragrightfromx = event.mouseButton.x;
				_dragrightfromy = event.mouseButton.y;
			}
			action.state = Window_Action_State::NOTHING;
			return action;
		}
		case sf::Event::MouseButtonReleased:
		{
			sf::Vector2f displayfrompos;
			sf::Vector2f displaytopos{
				(float)event.mouseButton.x * _scalex,
				(float)event.mouseButton.y * _scaley
			};

			action.tosquarecoord = BoardComponent::Coord{
				board.getDisplaySquareOffset(),
				board.getDisplaySquareSize(),
				displaytopos,
				_allowintersections
			};

			if (event.mouseButton.button == sf::Mouse::Left)
			{
				action.isLeftTool = true;
				displayfrompos = sf::Vector2f{
					(float)_dragleftfromx * _scalex,
					(float)_dragleftfromy * _scaley
				};

				action.fromsquarecoord = BoardComponent::Coord{
				board.getDisplaySquareOffset(),
				board.getDisplaySquareSize(),
				displayfrompos,
				_allowintersections
				};
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				action.isLeftTool = false;
				displayfrompos = sf::Vector2f{
					(float)_dragrightfromx * _scalex,
					(float)_dragrightfromy * _scaley
				};

				action.fromsquarecoord = BoardComponent::Coord{
					board.getDisplaySquareOffset(),
					board.getDisplaySquareSize(),
					displayfrompos,
					_allowintersections
				};
			}
			else
			{
				action.state = Window_Action_State::NOTHING;
				return action;
			}

			if (board.isWithinBoard(action.tosquarecoord))
			{
				if (toolwindow.getTool(action.isLeftTool).getState() == Tool_State::REMOVE_SQUARE)
				{
					action.state = Window_Action_State::REMOVE_SQUARE;
					return action;
				}
				
				if (board.isValidSquare(action.tosquarecoord))
				{
					switch (toolwindow.getTool(action.isLeftTool).getState())
					{
					case Tool_State::SELECT:
						if (action.fromsquarecoord == action.tosquarecoord)
						{
							action.state = Window_Action_State::REMOVE_DRAG_ARROW;
							return action;
						}
						else if (!(board.isEmptySquare(action.fromsquarecoord)))
						{
							action.state = Window_Action_State::MOVE_PIECE;
							return action;
						}
						action.state = Window_Action_State::REMOVE_DRAG_ARROW;
						return action;
					case Tool_State::SET_SQUARE_COLOR:
						action.state = Window_Action_State::SET_SQUARE_COLOR;
						return action;
					case Tool_State::REMOVE:
						action.state = Window_Action_State::REMOVE_PIECE;
						return action;
					case Tool_State::ADD_PIECE:
						action.state = Window_Action_State::ADD_PIECE;
						return action;
					case Tool_State::ADD_LINE:
						if (action.fromsquarecoord == action.tosquarecoord)
						{
							action.state = Window_Action_State::HIGHLIGHT_SQUARE;
							return action;
						}
						else
						{
							action.state = Window_Action_State::ADD_LINE;
							return action;
						}
					case Tool_State::ADD_ARROW:
						if (action.fromsquarecoord == action.tosquarecoord)
						{
							action.state = Window_Action_State::HIGHLIGHT_SQUARE;
							return action;
						}
						else
						{
							action.state = Window_Action_State::ADD_ARROW;
							return action;
						}
					case Tool_State::ADD_PIECE_ACCESSORY:
						if (board.isEmptySquare(action.tosquarecoord))
						{
							action.state = Window_Action_State::NOTHING;
							return action;
						}
						else
						{
							action.state = Window_Action_State::ADD_PIECE_ACCESSORY;
							return action;
						}
					case Tool_State::ADD_PIECE_CHAR_ACCESSORY:
						if (board.isEmptySquare(action.tosquarecoord))
						{
							action.state = Window_Action_State::NOTHING;
							return action;
						}
						else
						{
							action.state = Window_Action_State::ADD_PIECE_CHAR_ACCESSORY;
							return action;
						}
					case Tool_State::PIECE_PICKER:
						if (board.isEmptySquare(action.tosquarecoord))
						{
							action.toolstate = Tool_State::REMOVE;
							action.state = Window_Action_State::SET_TOOL_STATE;
							return action;
						}
						else
						{
							action.state = Window_Action_State::SET_TOOL_PIECE;
							action.piece = board.getPiece(action.tosquarecoord);
							return action;
						}
					}
				}
				else //Within Board, not valid square
				{
					switch (toolwindow.getTool(action.isLeftTool).getState())
					{
						case Tool_State::SELECT:
						{
							int id{ 0 };
							for (auto profilebox : board.getProfileBoxList())
							{
								if (profilebox.isWithinBorder(
									board.getImageCoordFromDisplayCoord(displaytopos).x,
									board.getImageCoordFromDisplayCoord(displaytopos).y))
								{
									action.player_id = id;
									action.state = Window_Action_State::HIGHLIGHT_PLAYERBOX;
									return action;
								}
								id++;
							}
							break;
						}
						case Tool_State::ADD_PROFILE_BOX:
						{
							int id{ 0 };
							for (auto profilebox : board.getProfileBoxList())
							{
								if (profilebox.isWithinBorder(
									board.getImageCoordFromDisplayCoord(displaytopos).x,
									board.getImageCoordFromDisplayCoord(displaytopos).y))
								{
									action.player_id = id;
									action.state = Window_Action_State::SET_PROFILE_BOX;
									return action;
								}
								id++;
							}
							action.state = Window_Action_State::ADD_PROFILE_BOX;
							return action;
						}
							
						case Tool_State::REMOVE:
						{
							int id{ 0 };
							for (auto profilebox : board.getProfileBoxList())
							{
								if (profilebox.isWithinBorder(
									board.getImageCoordFromDisplayCoord(displaytopos).x,
									board.getImageCoordFromDisplayCoord(displaytopos).y))
								{
									action.player_id = id;
									action.state = Window_Action_State::REMOVE_PROFILE_BOX;
									return action;
								}
								id++;
							}
							action.state = Window_Action_State::NOTHING;
							return action;
						}
							
						default:
							action.state = Window_Action_State::NOTHING;
							return action;
					}
				}
			}
			else if (toolwindow.isIconArea(ToolWindowComponent::LEFTTOOL,
				sf::Vector2f{ displaytopos.x,
					(displaytopos.y - board.getDisplaySize().y) }))
			{
				action.state = Window_Action_State::PICK_PIECE;
				return action;
			}
			else if (toolwindow.isIconArea(ToolWindowComponent::RIGHTTOOL,
				sf::Vector2f{displaytopos.x,
				(displaytopos.y - board.getDisplaySize().y) }))
			{
			action.state = Window_Action_State::SWAP_TOOLS;
			return action;
			}
			else if (toolwindow.isIconArea(ToolWindowComponent::ARROWLEFT,
				sf::Vector2f{ displaytopos.x,
				(displaytopos.y - board.getDisplaySize().y) }))
			{
				action.state = Window_Action_State::LOAD_BOARD_LEFT;
				return action;
			}
			else if (toolwindow.isIconArea(ToolWindowComponent::ARROWRIGHT,
				sf::Vector2f{ displaytopos.x,
				(displaytopos.y - board.getDisplaySize().y) }))
			{
				action.state = Window_Action_State::LOAD_BOARD_RIGHT;
				return action;
			}
			else if (toolwindow.isIconArea(ToolWindowComponent::ADDBOARD,
				sf::Vector2f{ displaytopos.x,
				(displaytopos.y - board.getDisplaySize().y) }))
			{
				action.state = Window_Action_State::ADD_BOARD;
				return action;
			}
			else if (toolwindow.isIconArea(ToolWindowComponent::SAVE,
				sf::Vector2f{ displaytopos.x,
				(displaytopos.y - board.getDisplaySize().y) }))
			{
				if (action.isLeftTool)
				{
					action.state = Window_Action_State::SAVE_BOARD;
					return action;
				}
				else
				{
					action.state = Window_Action_State::ENTER_SAVE_AS_BOARDNAME;
					return action;
				}
			}
			else if (toolwindow.isIconArea(ToolWindowComponent::REMOVEBOARD,
				sf::Vector2f{ displaytopos.x,
				(displaytopos.y - board.getDisplaySize().y) }))
			{
				action.state = Window_Action_State::DELETE_BOARD;
				return action;
			}
			action.state = Window_Action_State::REMOVE_DRAG_ARROW;
			return action;
		}
		case sf::Event::MouseMoved:
		{
			sf::Vector2f displayfrompos;
			sf::Vector2f displaytopos{
				(float)event.mouseMove.x * _scalex,
				(float)event.mouseMove.y * _scaley
			};

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				action.isLeftTool = true;
				displayfrompos = sf::Vector2f{
					(float)_dragleftfromx * _scalex,
					(float)_dragleftfromy * _scaley
				};

				action.fromsquarecoord = BoardComponent::Coord{
					board.getDisplaySquareOffset(),
					board.getDisplaySquareSize(),
					displayfrompos,
					_allowintersections
				};
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				action.isLeftTool = false;
				displayfrompos = sf::Vector2f{
					(float)_dragrightfromx * _scalex,
					(float)_dragrightfromy * _scaley
				};

				action.fromsquarecoord = BoardComponent::Coord{
					board.getDisplaySquareOffset(),
					board.getDisplaySquareSize(),
					displayfrompos,
					_allowintersections
				};
			}
			else
			{
				return action;
			}

			switch (toolwindow.getTool(action.isLeftTool).getState())
			{
			case Tool_State::SELECT:

				action.tosquarecoord = BoardComponent::Coord{
					board.getDisplaySquareOffset(),
					board.getDisplaySquareSize(),
					displaytopos,
					_allowintersections
				};
				if (board.isValidSquare(action.tosquarecoord) && !(board.isEmptySquare(action.fromsquarecoord)))
				{
					action.state = Window_Action_State::UPDATE_DRAG_ARROW;
					return action;
				}
				break;
			case Tool_State::ADD_LINE:
			case Tool_State::ADD_ARROW:
				action.tosquarecoord = BoardComponent::Coord{
					board.getDisplaySquareOffset(),
					board.getDisplaySquareSize(),
					displaytopos,
					_allowintersections
				};
				if (board.isValidSquare(action.tosquarecoord))
				{
					action.state = Window_Action_State::UPDATE_DRAG_ARROW;
					return action;
				}
				break;
			default:
				break;
			}
		}
		default:
			break;
	}
	return action;
}

void EventHandler::toggleGridSelect()
{
	_allowintersections = !_allowintersections;
}