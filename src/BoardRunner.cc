#include "BoardRunner.h"

using namespace std;
BoardRunner::BoardRunner(string setupfilename)
{
	_piecehandler.loadStyle("bulldog");
	_piecehandler.loadStyle("musketeer");
	_board.setPieceHandler(&_piecehandler);
	_toolwindow.setPieceHandler(&_piecehandler);
	if (_boardnamelist.loadNames(setupfilename))
	{
		_boardnumber = _boardnamelist.getBoardNumber();
		_numboards = _boardnamelist.getNumBoards();
		_boardname = _boardnamelist.getName(_boardnumber);
		_board.load(_boardname);
	}
	else
	{
		_boardnumber = 0;
		_numboards = 0;
		_boardname = "board";
		_board.initLoad("chess");
	}
	
	_toolwindow.load(_board);
	_mainwindow.create(
		sf::VideoMode
		{
			_board.getDisplaySize().x,
			_board.getDisplaySize().y + _toolwindow.getSize().y
		},
		_boardname);

	if (_mainwindow.getPosition().y < 0)
	{
		_mainwindow.setPosition(
			sf::Vector2i{ _mainwindow.getPosition().x, 0 });
	}
	_pieceselectwindow.load(_piecehandler, _board, _toolwindow.getTool(true));

	if (_numboards == 0)
	{
		_mainwindow.setTitle("board");
		_toolwindow.setSaveIconNotSaved();
	}

	_textbox.setPosition(sf::Vector2f{
		(float)_board.getDisplaySize().x / 2.f,
		(float)_board.getDisplaySize().y / 2.f });
}



BoardRunner::~BoardRunner()
{
}

void BoardRunner::run(std::default_random_engine & rng)
{
	sf::Clock delayclock;
	while (_mainwindow.isOpen())
	{
		sf::Event event;
		while (true)
		{
			if (_mainwindow.pollEvent(event))
			{
				_action = _event_handler.handleEvent(
					event,
					_board,
					_toolwindow,
					_keyboardhandler);
			}
			else if (_pieceselectwindow.pollEvent(event))
			{
				_action = _pieceselectwindow.handleEvent(
					event,
					_board,
					_toolwindow.getTool(true),
					_toolwindow.getTool(false),
					_piecehandler,
					_keyboardhandler
				);
			}
			else
			{
				break;
			}
			Tool* tool = &(_toolwindow.getTool(_action.isLeftTool));
			switch (_action.state)
			{
			case Window_Action_State::CLOSE_WINDOW:
				_pieceselectwindow.close();
				_mainwindow.close();
				_boardnamelist.save(_boardnumber);
				return;
			case Window_Action_State::CLOSE_PIECE_WINDOW:
				_pieceselectwindow.close();
				break;
			case Window_Action_State::SAVE_BOARD:
				if (_numboards > 0)
				{
					if (!(_toolwindow.isSaved()))
					{
						_board.save(_boardnamelist.getName(_boardnumber));
						_toolwindow.setSaveIconSaved();
					}
				}
				else
				{
					_numboards++;
					_boardnumber = _numboards;
					_boardnamelist.push_back("board");
					_board.save(_boardnamelist.getName(_boardnumber));
					_toolwindow.setSaveIconSaved();
					_mainwindow.setTitle(_boardnamelist.getName(_boardnumber));
				}
				break;
			
			case Window_Action_State::LOAD_BOARD:
				if (_action.board_number <= _numboards)
				{
					if (_board.load(_boardnamelist.getName(_action.board_number)))
					{
						_boardnumber = _action.board_number;
						_toolwindow.setSaveIconSaved();
						_mainwindow.setTitle(_boardnamelist.getName(_boardnumber));
					}
					else
					{
						_boardnamelist.remove(_action.board_number);
						_numboards--;
					}
				}
				break;
			case Window_Action_State::LOAD_BOARD_LEFT:
				if (_numboards > 0)
				{
					int newboardnumber = ((_numboards + _boardnumber - 2)
						% _numboards) + 1;
					if (_board.load(_boardnamelist.getName(newboardnumber)))
					{
						_boardnumber = newboardnumber;
						_toolwindow.setSaveIconSaved();
						_mainwindow.setTitle(_boardnamelist.getName(_boardnumber));
					}
					else
					{
						_boardnamelist.remove(newboardnumber);
						_numboards--;
					}
				}
				break;
			case Window_Action_State::LOAD_BOARD_RIGHT:
				if (_numboards > 0)
				{
					int newboardnumber = (_boardnumber % _numboards) + 1;
					if (_board.load(_boardnamelist.getName(newboardnumber)))
					{
						_boardnumber = newboardnumber;
						_toolwindow.setSaveIconSaved();
						_mainwindow.setTitle(_boardnamelist.getName(_boardnumber));
					}
					else
					{
						_boardnamelist.remove(newboardnumber);
						_numboards--;
					}
				}
				break;
			case Window_Action_State::ADD_BOARD:
			{
				_numboards++;
				_boardnumber = _numboards;
				string boardtype = "chess";
				_board.initLoad(boardtype);
				_boardnamelist.push_back("board");
				_board.save(_boardnamelist.getName(_boardnumber));
				_toolwindow.setSaveIconSaved();
				_mainwindow.setTitle(_boardnamelist.getName(_boardnumber));
				break;
			}
			case Window_Action_State::DELETE_BOARD:
				if (_numboards > 1)
				{
					_boardnamelist.remove(_boardnumber);
					_numboards--;
					_boardnumber = ((_boardnumber - 1) % _numboards) + 1;
					_board.load(_boardnamelist.getName(_boardnumber));
					_toolwindow.setSaveIconSaved();
					_mainwindow.setTitle(_boardnamelist.getName(_boardnumber));
				}
				else if (_numboards == 1)
				{
					_boardnamelist.remove(_boardnumber);
					_numboards--;
					_boardnumber = 0;
					_board.initLoad("chess");
					_toolwindow.setSaveIconNotSaved();
					_mainwindow.setTitle("board");
				}
				else
				{
					_board.initLoad("chess");
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::ADD_ROW_UP:
				if (_board.addRowUp())
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::ADD_ROW_DOWN:
				if (_board.addRowDown())
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::ADD_COLUMN_LEFT:
				if (_board.addColumnLeft())
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::ADD_COLUMN_RIGHT:
				if (_board.addColumnRight())
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::REMOVE_ROW_UP:
				if (_board.removeRowUp())
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::REMOVE_ROW_DOWN:
				if (_board.removeRowDown())
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::REMOVE_COLUMN_LEFT:
				if (_board.removeColumnLeft())
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::REMOVE_COLUMN_RIGHT:
				if (_board.removeColumnRight())
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::ADD_PROFILE_BOX:
				if(_board.addProfileBox(tool->getProfileBox() ,_action.tosquarecoord))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
					_toolwindow.setTool(_action.isLeftTool, Tool_State::SELECT);
					_toolwindow.updateToolImage(_action.isLeftTool);
				}
				_pieceselectwindow.update(_piecehandler, _board, *tool);
				break;
			case Window_Action_State::REMOVE_PROFILE_BOX:
				if (_board.removeProfileBox(_action.player_id))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
					_toolwindow.setTool(_action.isLeftTool, Tool_State::SELECT);
					_toolwindow.updateToolImage(_action.isLeftTool);
				}
				break;
			case Window_Action_State::SET_PROFILE_BOX:
				if (_board.updateProfileBox(_action.player_id, tool->getPlayerName(), tool->getBorderColor()))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}

				_toolwindow.setTool(_action.isLeftTool, Tool_State::SELECT);
				_toolwindow.updateToolImage(_action.isLeftTool);
				break;
			case Window_Action_State::SET_TOOL_STATE:
				_toolwindow.setTool(_action.isLeftTool, _action.toolstate);
				_toolwindow.updateToolImage(_action.isLeftTool);
				_pieceselectwindow.update(_piecehandler, _board, *tool);
				break;
			case Window_Action_State::SET_TOOL_PIECE_STYLE:
				tool->getPieceBrush().setStyle(_action.name);
				if ((!tool->getPieceBrush().exists()))
				{
					tool->getPieceBrush().setName("pawn");
				}
				tool->getPieceBrush().setScale(_piecehandler.getScale(tool->getPieceBrush().getStyle()));
				_pieceselectwindow.update(_piecehandler, _board, *tool);
				_toolwindow.setTool(_action.isLeftTool, Tool_State::ADD_PIECE);
				_toolwindow.updateToolImage(_action.isLeftTool);
				break;
			case Window_Action_State::SET_TOOL_PIECE_TYPE:
				tool->getPieceBrush().setName(_action.piece.getName());
				_pieceselectwindow.update(_piecehandler, _board, *tool);

				_toolwindow.setTool(_action.isLeftTool, Tool_State::ADD_PIECE);
				_toolwindow.updateToolImage(_action.isLeftTool);
				break;
			case Window_Action_State::SET_TOOL_COLOR:
				tool->setColorIndex(_action.colorindex);
				_pieceselectwindow.update(_piecehandler, _board, *tool);

				switch (tool->getState())
				{
				case Tool_State::ADD_LINE:
				case Tool_State::ADD_ARROW:
				case Tool_State::ADD_PIECE:
				case Tool_State::ADD_PROFILE_BOX:
				case Tool_State::SET_SQUARE_COLOR:
				case Tool_State::ADD_PIECE_ACCESSORY:
				case Tool_State::ADD_PIECE_CHAR_ACCESSORY:
					break;
				default:
					_toolwindow.setTool(_action.isLeftTool, Tool_State::ADD_PIECE);
					break;
				}
				_toolwindow.updateToolImage(_action.isLeftTool);
				break;
			case Window_Action_State::SET_TOOL_PIECE:
				tool->getPieceBrush() = _action.piece;

				_toolwindow.setTool(_action.isLeftTool, Tool_State::ADD_PIECE);
				_toolwindow.updateToolImage(_action.isLeftTool);
				_pieceselectwindow.update(_piecehandler, _board, *tool);
				break;
			case Window_Action_State::SET_TOOL_ADD_PIECE_ACCESSORY:
				_toolwindow.setTool(_action.isLeftTool, Tool_State::ADD_PIECE_ACCESSORY);
				tool->setAccessoryName(_action.name);
				_toolwindow.updateToolImage(_action.isLeftTool);
				_pieceselectwindow.update(_piecehandler, _board, *tool);
				break;
			case Window_Action_State::SWAP_TOOLS:
				_toolwindow.swapTools();
				_toolwindow.updateToolImage(true);
				_toolwindow.updateToolImage(false);
				_pieceselectwindow.update(_piecehandler, _board, *tool);
			case Window_Action_State::ADD_PIECE:
				if (_board.addPiece(tool->getPieceBrush(),
					_action.tosquarecoord))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::REMOVE_PIECE:
				if (_board.removePiece(_action.tosquarecoord))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::MOVE_PIECE:
				if (_board.movePiece(
					_action.fromsquarecoord,
					_action.tosquarecoord))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				_board.removeDragArrow();
				break;
			case Window_Action_State::FLIP_PIECE_HORIZONTAL:
				tool->flipPieceHorizontally();
				_pieceselectwindow.update(_piecehandler, _board, *tool);

				_toolwindow.setTool(_action.isLeftTool, Tool_State::ADD_PIECE);
				_toolwindow.updateToolImage(_action.isLeftTool);
				break;
			case Window_Action_State::FLIP_PIECE_VERTICAL:
				tool->flipPieceVertically();
				_pieceselectwindow.update(_piecehandler, _board, *tool);

				_toolwindow.setTool(_action.isLeftTool, Tool_State::ADD_PIECE);
				_toolwindow.updateToolImage(_action.isLeftTool);
				break;
			case Window_Action_State::INVERT_PIECE_COLOR:
				tool->invertPieceColor();
				_pieceselectwindow.update(_piecehandler, _board, *tool);

				_toolwindow.setTool(_action.isLeftTool, Tool_State::ADD_PIECE);
				_toolwindow.updateToolImage(_action.isLeftTool);
				break;
			case Window_Action_State::SET_SQUARE_COLOR:
				if (_board.setSquareColor(
					_action.tosquarecoord,
					tool->getSquareColor()))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::SET_PIECE_ACCESSORY:
				tool->setAccessoryName(_action.name);
				if (tool->getState() == Tool_State::ADD_PIECE_ACCESSORY)
				{
					_toolwindow.updateToolImage(_action.isLeftTool);
				}
				_pieceselectwindow.update(_piecehandler, _board, *tool);
				break;
			case Window_Action_State::SET_PIECE_CHAR_ACCESSORY:
				tool->setCharAccessory(_action.name.at(0));
				if (tool->getState() == Tool_State::ADD_PIECE_CHAR_ACCESSORY)
				{
					_toolwindow.updateToolImage(_action.isLeftTool);
				}
				_pieceselectwindow.update(_piecehandler, _board, *tool);
				break;
			case Window_Action_State::REMOVE_SQUARE:
				if (_board.removeSquare(_action.tosquarecoord))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::ADD_LINE:
				_board.removeDragArrow();
				if (_board.addLine(
					_action.fromsquarecoord,
					_action.tosquarecoord,
					tool->getArrowColor()))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::ADD_ARROW:
				_board.removeDragArrow();
				if (_board.addArrow(
					_action.fromsquarecoord,
					_action.tosquarecoord,
					tool->getArrowColor()))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::ADD_PIECE_ACCESSORY:
				_board.removeDragArrow();
				if (_board.addPieceAccessory(_action.tosquarecoord, tool->getAccessoryName(), tool->getAccessoryColor()))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::ADD_PIECE_CHAR_ACCESSORY:
				_board.removeDragArrow();
				if (_board.addPieceAccessory(
					_action.tosquarecoord,
					string( 1, tool->getCharAccessory() ),
					tool->getAccessoryColor()))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::HIGHLIGHT_SQUARE:
				if (_board.addHighlightSquare(
					_action.tosquarecoord,
					tool->getArrowColor()))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
				case Window_Action_State::HIGHLIGHT_PLAYERBOX:
					if (_board.highlightPlayerBox(_action.player_id))
					{
						_board.updateImage();
						_toolwindow.setSaveIconNotSaved();
					}
					break;
			case Window_Action_State::UPDATE_DRAG_ARROW:
				_board.updateDragArrow(
					_action.fromsquarecoord,
					_action.tosquarecoord);
				break;
			case Window_Action_State::REMOVE_DRAG_ARROW:
				_board.removeDragArrow();
				break;
			case Window_Action_State::PICK_PIECE:
			{
				if (!(_pieceselectwindow.isOpen()))
				{
					_pieceselectwindow.open(_mainwindow);
				}
				else
				{
					_pieceselectwindow.close();
				}
				break;
			}
			case Window_Action_State::TOGGLE_GRID:
				if (_board.togglegrid())
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::GENERATE_COLORED_BOARD:
				if (_board.generateColoredBoard(rng))
				{
					_board.updateImage();
					_toolwindow.setSaveIconNotSaved();
				}
				break;
			case Window_Action_State::ENTER_BOARD_NAME:
				_keyboardhandler.setState(Keyboard::State::ENTER_BOARD_NAME);
				_mainwindow.setTitle("");
				_textbox.setText("Enter a new boardname:\n");
				_textbox.showText();
				break;
			case Window_Action_State::DISPLAY_ENTERED_BOARD_NAME:
				_mainwindow.setTitle(_action.name);
				_textbox.setText("Enter a new boardname:\n" + _action.name);
				break;
			case Window_Action_State::SET_BOARD_NAME:
			{
				if (_numboards > 0)
				{
					string oldname = _boardnamelist.getName(_boardnumber);
					if (_boardnamelist.setName(_boardnumber, _action.name))
					{
						_board.newNameSave(_boardnamelist.getName(_boardnumber), oldname);
						_toolwindow.setSaveIconSaved();
					}
				}
				else
				{
					_numboards++;
					_boardnumber = _numboards;
					_boardnamelist.push_back(_action.name);
					_board.save(_boardnamelist.getName(_boardnumber));
					_toolwindow.setSaveIconSaved();
				}
				_mainwindow.setTitle(_boardnamelist.getName(_boardnumber));
				_textbox.setText("");
				_textbox.hideText();
			}
				break;
			case Window_Action_State::ENTER_PROFILE_NAME:
				_keyboardhandler.setState(Keyboard::State::ENTER_PROFILE_NAME);
				_textbox.setText("Enter playername:\n");
				_textbox.showText();
				break;
			case Window_Action_State::DISPLAY_ENTERED_PROFILE_NAME:
				_textbox.setText("Enter playername:\n" + _action.name);
				break;
			case Window_Action_State::SET_PROFILE_IMAGE:
				tool->setPlayerName(_action.name);
				_textbox.setText("");
				_textbox.hideText();
				break;
			case Window_Action_State::ENTER_SAVE_AS_BOARDNAME:
				_keyboardhandler.setState(Keyboard::State::ENTER_SAVE_AS_BOARDNAME);
				_textbox.setText("Save as:\n");
				_textbox.showText();
				break;
			case Window_Action_State::DISPLAY_SAVE_AS_BOARDNAME:
				_textbox.setText("Save as:\n" + _action.name);
				break;
			case Window_Action_State::SAVE_BOARD_AS:
				if (_action.name.size() > 0)
				{
					_numboards++;
					_boardnumber = _numboards;
					_boardnamelist.push_back(_action.name);
					_board.save(_boardnamelist.getName(_boardnumber));
					_toolwindow.setSaveIconSaved();
				}
				_mainwindow.setTitle(_boardnamelist.getName(_boardnumber));
				_textbox.setText("");
				_textbox.hideText();
				break;
			default:
				break;
			}
		}

		_mainwindow.clear(sf::Color{ 180, 180, 180, 255 });
		_mainwindow.draw(_board);
		_mainwindow.draw(_textbox);
		_mainwindow.draw(_toolwindow);
		_mainwindow.display();

		_pieceselectwindow.clear();
		_pieceselectwindow.draw();
		_pieceselectwindow.display();

		sf::sleep(sf::milliseconds(30) - delayclock.getElapsedTime());
		delayclock.restart();
	}
	_boardnamelist.save(_boardnumber);
}