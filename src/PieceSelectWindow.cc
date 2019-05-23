#include "PieceSelectWindow.h"

using namespace std;

PieceSelectWindow::PieceSelectWindow()
{
	initToolList();
	//must run load(board, toolwindow) to work
}

PieceSelectWindow::PieceSelectWindow(PieceHandler& piecehandler, Board& board, Tool& tool)
	:PieceSelectWindow::PieceSelectWindow{}
{
	load(piecehandler, board, tool);
}

void PieceSelectWindow::load(PieceHandler& piecehandler, Board& board, Tool& tool)
{
	_displaysquaresize = board.getImageSquareSize();
	_numcolumns = 5;
	_numrows = 14;
	_backgroundcolor = sf::Color{ 180, 180, 180 };

	_rendertexture.create(_displaysquaresize.x*_numcolumns, _displaysquaresize.y*_numrows);
	update(piecehandler, board, tool);
}

bool PieceSelectWindow::isOpen()
{
	return _window.isOpen();
}

void PieceSelectWindow::update(PieceHandler& piecehandler, Board& board, Tool& tool)
{
	_rendertexture.clear(_backgroundcolor);
	for (unsigned int y{ 0 }; y < _numrows; y++)
	{
		for (unsigned int x{ 0 }; x < _numcolumns; x++)
		{
			drawTool(piecehandler, tool, x, y);
		}
	}
	_rendertexture.display();
}

void PieceSelectWindow::open(sf::RenderWindow & window)
{
	_window.create( sf::VideoMode
		{
			_rendertexture.getSize().x,
			_rendertexture.getSize().y
		},
		"Pieces"
		);
	int xpos = window.getPosition().x - _window.getSize().x;
	int ypos = window.getPosition().y;
	_window.setPosition(sf::Vector2i{xpos, ypos});
}

void PieceSelectWindow::close()
{
	_window.close();
}



bool PieceSelectWindow::pollEvent(sf::Event & event)
{
	return _window.pollEvent(event);
}

Window_Action PieceSelectWindow::handleEvent(
	sf::Event event, Board& board,
	Tool& lefttool, Tool& righttool,
	PieceHandler& piecehandler,
	KeyboardHandler& keyboardhandler)
{
	Window_Action action;
	Tool* tool;
	switch (event.type)
	{
	case sf::Event::Closed:
		action.state = Window_Action_State::CLOSE_PIECE_WINDOW;
		return action;
	case sf::Event::KeyPressed:
		return keyboardhandler.handleKeyPress(event.key.code);
	case sf::Event::TextEntered:
		return keyboardhandler.handleTextEntered(event.text.unicode);
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			action.isLeftTool = true;
			tool = &lefttool;
		}
		else if (event.mouseButton.button == sf::Mouse::Right)
		{
			action.isLeftTool = false;
			tool = &righttool;
		}
		else
		{
			action.state = Window_Action_State::NOTHING;
			return action;
		}

		int x = event.mouseButton.x * _rendertexture.getSize().x /
			_window.getSize().x / _displaysquaresize.x;
		int y = event.mouseButton.y * _rendertexture.getSize().y /
			_window.getSize().y / _displaysquaresize.y;

		if (y < 0 || y >= (int)_numrows || x < 0 || x >= (int)_numcolumns)
		{
			action.state = Window_Action_State::NOTHING;
			return action;
		}
		int id = y * _numcolumns + x;
		switch (_toollist.at(id).tool)
		{
		case Piece_Tool::SELECT:
			action.toolstate = Tool_State::SELECT;
			action.state = Window_Action_State::SET_TOOL_STATE;
			return action;
		case Piece_Tool::REMOVE:
			action.toolstate = Tool_State::REMOVE;
			action.state = Window_Action_State::SET_TOOL_STATE;
			return action;
		case Piece_Tool::FLIP_HORIZONTAL:
			action.state = Window_Action_State::FLIP_PIECE_HORIZONTAL;
			return action;
		case Piece_Tool::FLIP_VERTICAL:
			action.state = Window_Action_State::FLIP_PIECE_VERTICAL;
			return action;
		case Piece_Tool::INVERT_COLORS:
			action.state = Window_Action_State::INVERT_PIECE_COLOR;
			return action;
		case Piece_Tool::MUSKETEER_STYLE:
			action.state = Window_Action_State::SET_TOOL_PIECE_STYLE;
			action.name = "musketeer";
			return action;
		case Piece_Tool::ADD_LINE:
			action.toolstate = Tool_State::ADD_LINE;
			action.state = Window_Action_State::SET_TOOL_STATE;
			return action;
		case Piece_Tool::ADD_ARROW:
			action.toolstate = Tool_State::ADD_ARROW;
			action.state = Window_Action_State::SET_TOOL_STATE;
			return action;
		case Piece_Tool::REMOVE_SQUARE:
			action.toolstate = Tool_State::REMOVE_SQUARE;
			action.state = Window_Action_State::SET_TOOL_STATE;
			return action;
		case Piece_Tool::COLOR_SQUARE:
			action.toolstate = Tool_State::SET_SQUARE_COLOR;
			action.state = Window_Action_State::SET_TOOL_STATE;
			return action;
		case Piece_Tool::ADD_PROFILE_BOX:
			action.toolstate = Tool_State::ADD_PROFILE_BOX;
			action.state = Window_Action_State::SET_TOOL_STATE;
			return action;
		case Piece_Tool::ADD_ACCESSORY:
			action.state = Window_Action_State::SET_TOOL_ADD_PIECE_ACCESSORY;
			action.name = _accessorynames[_toollist.at(id).id];
			return action;
		case Piece_Tool::ADD_CHAR_ACCESSORY:
			action.toolstate = Tool_State::ADD_PIECE_CHAR_ACCESSORY;
			action.state = Window_Action_State::SET_TOOL_STATE;
			return action;
		case Piece_Tool::SET_COLOR:
			action.colorindex = _toollist.at(id).id;
			action.state = Window_Action_State::SET_TOOL_COLOR;
			return action;
		case Piece_Tool::ADD_PIECE:
			action.piece = getPieceFromId(piecehandler, *tool, _toollist.at(id).id);
			if (action.piece.getName() != "" && action.piece.getColor().name != "")
			{
				action.state = Window_Action_State::SET_TOOL_PIECE;
				return action;
			}
			else
			{
				action.toolstate = Tool_State::REMOVE;
				action.state = Window_Action_State::SET_TOOL_STATE;
				return action;
			}
		default:
			action.toolstate = Tool_State::REMOVE;
			action.state = Window_Action_State::SET_TOOL_STATE;
			return action;
		}
	}
	action.state = Window_Action_State::NOTHING;
	return action;
}

void PieceSelectWindow::clear()
{
	_window.clear();
}

void PieceSelectWindow::draw()
{
	_window.draw(sf::Sprite{ _rendertexture.getTexture() });
}

void PieceSelectWindow::display()
{
	_window.display();
}

void PieceSelectWindow::initToolList()
{
	_toollist.push_back(Container{ Piece_Tool::SELECT, 0 });
	_toollist.push_back(Container{ Piece_Tool::REMOVE, 0 });
	_toollist.push_back(Container{ Piece_Tool::REMOVE_SQUARE, 0 });
	_toollist.push_back(Container{ Piece_Tool::COLOR_SQUARE, 0 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PROFILE_BOX, 0 });
	_toollist.push_back(Container{ Piece_Tool::MUSKETEER_STYLE, 0 });
	_toollist.push_back(Container{ Piece_Tool::ADD_LINE, 0 });
	_toollist.push_back(Container{ Piece_Tool::ADD_ARROW, 0 });
	_toollist.push_back(Container{ Piece_Tool::FLIP_HORIZONTAL, 0 });
	_toollist.push_back(Container{ Piece_Tool::FLIP_VERTICAL, 0 });
	_toollist.push_back(Container{ Piece_Tool::INVERT_COLORS, 0 });
	_toollist.push_back(Container{ Piece_Tool::ADD_ACCESSORY, 0 });
	_toollist.push_back(Container{ Piece_Tool::ADD_ACCESSORY, 1 });
	_toollist.push_back(Container{ Piece_Tool::ADD_ACCESSORY, 2 });
	_toollist.push_back(Container{ Piece_Tool::ADD_CHAR_ACCESSORY, 0 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 0 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 0 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 1 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 2 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 3 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 1 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 4 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 5 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 6 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 7 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 2 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 8 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 9 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 10 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 11 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 3 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 12 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 13 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 14 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 15 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 4 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 16 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 17 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 18 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 19 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 5 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 20 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 21 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 22 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 23 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 6 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 24 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 25 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 26 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 27 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 7 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 28 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 29 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 30 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 31 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 8 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 32 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 33 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 34 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 35 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 9 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 36 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 37 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 38 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 39 });
	_toollist.push_back(Container{ Piece_Tool::SET_COLOR, 10 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 40 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 41 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 42 });
	_toollist.push_back(Container{ Piece_Tool::ADD_PIECE, 43 });
}

string PieceSelectWindow::getTypeFromId(PieceHandler& piecehandler, Tool& tool, unsigned int index) const
{	
	if (index < piecehandler.getNumPieces(tool.getPieceBrush().getStyle()))
	{
		return piecehandler.getName(tool.getPieceBrush().getStyle(), index);
	}
	else
	{
		return "";
	}
}

Piece PieceSelectWindow::getPieceFromId(PieceHandler& piecehandler, Tool& tool, unsigned int index) const
{
	Piece piece{ tool.getPieceBrush() };
	piece.setName(getTypeFromId(piecehandler, tool, index));
	piece.removePieceOnTop();
	return piece;
}

string PieceSelectWindow::getToolFileName(Piece_Tool::Piece_Tool tool) const
{
	return "resources/icons/" + _toolnames.at(tool) + ".png";
}


void PieceSelectWindow::drawTool(PieceHandler& piecehandler, Tool& tool, unsigned int x, unsigned int y)
{
	auto square = getEmptySquare(x, y);
	_rendertexture.draw(square);

	sf::Vector2f position{
		float(_displaysquaresize.x*x),
		float(_displaysquaresize.y*y)
	};

	unsigned int id = y * _numcolumns + x;
	if (id < _toollist.size())
	{
		switch (_toollist.at(id).tool)
		{
		case Piece_Tool::ADD_LINE:
			tool.drawLineTool(_rendertexture, position,
				_displaysquaresize, tool.getColorIndex());
			return;
		case Piece_Tool::ADD_ARROW:
		{
			tool.drawArrowTool(_rendertexture, position,
				_displaysquaresize, tool.getColorIndex());
			return;
		}
		case Piece_Tool::COLOR_SQUARE:
			tool.drawSetSquareColorTool(_rendertexture, position,
				_displaysquaresize, tool.getColorIndex());
			return;
		case Piece_Tool::ADD_PROFILE_BOX:
			tool.drawAddProfileBoxTool(_rendertexture, position,
				_displaysquaresize, tool.getColorIndex());
			return;
		case Piece_Tool::REMOVE_SQUARE:
			tool.drawRemoveSquareTool(_rendertexture, position,
				_displaysquaresize);
			return;
		case Piece_Tool::INVERT_COLORS:
		{
			Piece piece = tool.getPieceBrush();
			piece.invertColors();
			piece.setSize(_displaysquaresize);
			piece.setPosition(position);
			_rendertexture.draw(piece);
			return;
		}
		case Piece_Tool::MUSKETEER_STYLE:
		{
			Piece piece = tool.getPieceBrush();
			if (tool.getPieceBrush().getStyle() != "musketeer")
			{
				piece.setStyle("musketeer");
			}
			else
			{
				piece.setStyle("bulldog");
			}
			if (!piece.exists())
			{
				piece.setName("pawn");
			}
			piece.setSize(_displaysquaresize);
			piece.setPosition(position);
			_rendertexture.draw(piece);
			return;
		}
		case Piece_Tool::ADD_ACCESSORY:
			tool.drawAccessoryTool(
				_rendertexture, position, _displaysquaresize,
				tool.getColorIndex(), _accessorynames[_toollist.at(id).id]);
			return;
		case Piece_Tool::ADD_CHAR_ACCESSORY:
			tool.drawAccessoryTool(
				_rendertexture, position, _displaysquaresize,
				tool.getColorIndex(), string(1, tool.getCharAccessory()));
			return;
		case Piece_Tool::SET_COLOR:
			switch (tool.getState())
			{
			case Tool_State::ADD_LINE:
				tool.drawLineTool(_rendertexture, position,
					_displaysquaresize, _toollist.at(id).id);
				return;
			case Tool_State::ADD_ARROW:
				tool.drawArrowTool(_rendertexture, position,
					_displaysquaresize, _toollist.at(id).id);
				return;
			case Tool_State::SET_SQUARE_COLOR:
				tool.drawSetSquareColorTool(_rendertexture, position,
					_displaysquaresize, _toollist.at(id).id);
				return;
			case Tool_State::ADD_PROFILE_BOX:
				tool.drawAddProfileBoxTool(_rendertexture, position,
					_displaysquaresize, _toollist.at(id).id);
				return;
			case Tool_State::ADD_PIECE_ACCESSORY:
				tool.drawAccessoryTool(_rendertexture, position,
					_displaysquaresize, _toollist.at(id).id, tool.getAccessoryName());
				return;
			case Tool_State::ADD_PIECE_CHAR_ACCESSORY:
				tool.drawAccessoryTool(_rendertexture, position,
					_displaysquaresize, _toollist.at(id).id, string(1, tool.getCharAccessory()));
				return;
			default:
				Piece piece = tool.getPieceBrush();
				piece.setColor(tool.getPieceColor(_toollist.at(id).id));
				piece.setSize(_displaysquaresize);
				piece.setPosition(position);
				_rendertexture.draw(piece);
				return;
			}
		case Piece_Tool::ADD_PIECE:
		{
			Piece piece = tool.getPieceBrush();
			piece.setName(getTypeFromId(piecehandler, tool, _toollist.at(id).id));
			if (piece.exists())
			{
				piece.setSize(_displaysquaresize);
				piece.setPosition(position);
				_rendertexture.draw(piece);
			}
			return;
		}
		default:
			string imagefilename = getToolFileName(_toollist.at(id).tool);
			sf::Image icon;
			sf::Color squarecolor = getSquareColor(x, y);
			sf::Image toolimage;
			toolimage.create(_displaysquaresize.x, _displaysquaresize.y, squarecolor);
			if (std::experimental::filesystem::exists(imagefilename))
			{
				icon.loadFromFile(imagefilename);
			}
			else
			{
				icon.loadFromFile(getToolFileName(Piece_Tool::REMOVE));
			}
			toolimage.copy(icon, 0, 0, sf::IntRect(0, 0, 0, 0), true);
			sf::Texture texture;
			texture.loadFromImage(toolimage);
			sf::Sprite sprite{ texture };
			sprite.setPosition(position);
			_rendertexture.draw(sprite);
			return;
		}
	}
}

sf::Color PieceSelectWindow::getSquareColor(unsigned int x, unsigned int y) const
{
	if ((x + y) % 2 == 0)
	{
		return sf::Color{ 118, 150, 85, 255 };
	}
	else
	{
		return sf::Color{ 239, 238, 210, 255 };
	}
}

unsigned int PieceSelectWindow::getToolIndex(unsigned int x, unsigned int y) const
{
	return y * _numcolumns + x;
}

sf::RectangleShape PieceSelectWindow::getEmptySquare(unsigned int x, unsigned int y) const
{
	sf::RectangleShape square{
				sf::Vector2f{
					(float)_displaysquaresize.x,
					(float)_displaysquaresize.y }
	};
	square.setFillColor(getSquareColor(x, y));
	square.setPosition(sf::Vector2f{
			(float)(_displaysquaresize.x*x),
			(float)(_displaysquaresize.y*y)
		}
	);
	return square;
}