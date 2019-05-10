#include "PieceSelectWindow.h"

using namespace std;

PieceSelectWindow::PieceSelectWindow()
{
	//must run load(board, toolwindow) to work
}

PieceSelectWindow::PieceSelectWindow(Board & board, Tool & tool)
{
	load(board, tool);
}

void PieceSelectWindow::load(Board & board, Tool & tool)
{
	_squaresize = board.getImageSquareSize();
	_numcolumns = 5;
	_backgroundcolor = sf::Color{ 180, 180, 180 };
	_numtoolrows = (_tools.size() + _numcolumns - 1) / _numcolumns;

	int numpiecerows = max(
		tool.getNumColors(),
		(_pieces.size() + _numcolumns - 2) / (_numcolumns - 1));

	_numrows = _numtoolrows + numpiecerows;

	_rendertexture.create(_squaresize.x*_numcolumns, _squaresize.y*_numrows);

	update(board, tool);
	}

bool PieceSelectWindow::isOpen()
{
	return _window.isOpen();
}

void PieceSelectWindow::update(Board & board, Tool & tool)
{
	_rendertexture.clear(_backgroundcolor);
	for (unsigned int y{ 0 }; y < _numrows; y++)
	{
		for (unsigned int x{ 0 }; x < _numcolumns; x++)
		{
			drawTool(tool, x, y);
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
	sf::Event event, Board & board,
	Tool & lefttool, Tool & righttool,
	Keyboardhandler & keyboardhandler)
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
			_window.getSize().x / _squaresize.x;
		int y = event.mouseButton.y * _rendertexture.getSize().y /
			_window.getSize().y / _squaresize.y;

		if (y < 0 || y >= (int)_numrows || x < 0 || x >= (int)_numcolumns)
		{
			action.state = Window_Action_State::NOTHING;
			return action;
		}
		else if (y < (int)_numtoolrows) //clicked on a tool
		{
			switch (getPieceToolFromId(x, y))
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
			case Piece_Tool::ACCESSORY_STAR6POINT:
				action.state = Window_Action_State::SET_TOOL_ADD_PIECE_ACCESSORY;
				action.name = "star6point";
				return action;
			case Piece_Tool::ACCESSORY_SHIELD:
				action.state = Window_Action_State::SET_TOOL_ADD_PIECE_ACCESSORY;
				action.name = "shield";
				return action;
			case Piece_Tool::ACCESSORY_SWORD:
				action.state = Window_Action_State::SET_TOOL_ADD_PIECE_ACCESSORY;
				action.name = "sword";
				return action;
			case Piece_Tool::ACCESSORY_CHAR:
				action.toolstate = Tool_State::ADD_PIECE_CHAR_ACCESSORY;
				action.state = Window_Action_State::SET_TOOL_STATE;
				return action;
			default:
				action.toolstate = Tool_State::REMOVE;
				action.state = Window_Action_State::SET_TOOL_STATE;
				return action;
			}
		}
		else if (x == 0) //clicked on a color
		{
			action.colorindex = y - _numtoolrows;
			action.state = Window_Action_State::SET_TOOL_COLOR;
			return action;
		}
		else //clicked on piece
		{
			action.piece = getPieceFromId(*tool, x, y);
			if(action.piece.getName() != "" && action.piece.getColor().name != "")
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

PieceColor PieceSelectWindow::getColorFromId(Tool & tool, int x, int y) const
{
	if(x != 0)
	{
		return tool.getPieceBrush().getColor();
	}
	unsigned int index = y - _numtoolrows;
	if(index < tool.getNumColors())
	{
		return tool.getPieceColor(index);
	}
	else
	{
		return tool.getPieceColor(0);
	}
}

string PieceSelectWindow::getTypeFromId(Tool & tool, int x, int y) const
{	
	if (x == 0)
	{
		return tool.getPieceBrush().getName();
	}

	unsigned int index = ((y - _numtoolrows) * (_numcolumns - 1) + ((x-1) % (_numcolumns - 1)));
		
	if (index < _pieces.size())
	{
		return _pieces.at(index);
	}
	else
	{
		return "";
	}
}

Piece PieceSelectWindow::getPieceFromId(Tool & tool, int x, int y) const
{
	Piece piece{ tool.getPieceBrush() };
	if (x == 0)
	{
		piece.setColor(getColorFromId(tool, x, y));
	}
	else
	{
		piece.setName(getTypeFromId(tool, x, y));
	}
	piece.removePieceOnTop();
	return piece;
}

Piece_Tool::Piece_Tool PieceSelectWindow::getPieceToolFromId(unsigned int index)
{
	if (index < _tools.size())
	{
		return _tools.at(index);
	}
	else
	{
		return Piece_Tool::INVALID;
	}
}

Piece_Tool::Piece_Tool PieceSelectWindow::getPieceToolFromId(int x, int y)
{
	int index = (y * _numcolumns + (x % _numcolumns));
	return getPieceToolFromId(index);
}

string PieceSelectWindow::getToolName(Piece_Tool::Piece_Tool piece_tool) const
{
	return _toolnames.at(piece_tool);
}

string PieceSelectWindow::getToolName(int index) const
{
	return getToolName(_tools.at(index));
}

string PieceSelectWindow::getToolName(int x, int y) const
{
	int index = (y * _numcolumns + (x % _numcolumns));
	return getToolName(index);
}

string PieceSelectWindow::getToolFileName(int index) const
{
	return "resources/icons/" + getToolName(index) + ".png";
}

void PieceSelectWindow::drawTool(Tool & tool, unsigned int x, unsigned int y)
{
	auto square = getEmptySquare(x, y);
	_rendertexture.draw(square);

	sf::Color squarecolor = getSquareColor(x, y);
	unsigned int toolindex = getToolIndex(x, y);
	sf::Vector2f position{
		float(_squaresize.x*x),
		float(_squaresize.y*y)
	};

	sf::Image toolimage;
	if (toolindex < _tools.size())
	{
		switch (_tools.at(toolindex))
		{
		case Piece_Tool::ADD_ARROW:
		{
			tool.drawArrowTool(_rendertexture, position,
				_squaresize, tool.getColorIndex());
			return;
		}
		case Piece_Tool::COLOR_SQUARE:
			tool.drawSetSquareColorTool(_rendertexture, position,
				_squaresize, tool.getColorIndex());
			return;
		case Piece_Tool::ADD_PROFILE_BOX:
			tool.drawAddProfileBoxTool(_rendertexture, position,
				_squaresize, tool.getColorIndex());
			return;
		case Piece_Tool::REMOVE_SQUARE:
			tool.drawRemoveSquareTool(_rendertexture, position,
				_squaresize);
			return;
		case Piece_Tool::INVERT_COLORS:
		{
			Piece piece = tool.getPieceBrush();
			piece.invertColors();
			piece.setSize(_squaresize);
			piece.draw(_rendertexture, position);
			return;
		}
		case Piece_Tool::ACCESSORY_STAR6POINT:
			tool.drawAccessoryTool(
				_rendertexture, position, _squaresize,
				tool.getColorIndex(), "star6point");
			return;
		case Piece_Tool::ACCESSORY_SHIELD:
			tool.drawAccessoryTool(
				_rendertexture, position, _squaresize,
				tool.getColorIndex(), "shield");
			return;
		case Piece_Tool::ACCESSORY_SWORD:
			tool.drawAccessoryTool(
				_rendertexture, position, _squaresize,
				tool.getColorIndex(), "sword");
			return;
		case Piece_Tool::ACCESSORY_CHAR:
			tool.drawAccessoryTool(
				_rendertexture, position, _squaresize,
				tool.getColorIndex(), string(1, tool.getCharAccessory()));
			return;
		default:
			string imagefilename = getToolFileName(toolindex);
			sf::Image icon;
			toolimage.create(_squaresize.x, _squaresize.y, squarecolor);
			if (std::experimental::filesystem::exists(imagefilename))
			{
				icon.loadFromFile(imagefilename);
			}
			else
			{
				icon.loadFromFile(getToolFileName(0));
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
	else if (y >= _numtoolrows)
	{
		if (x == 0)
		{
			switch (tool.getState())
			{
			case Tool_State::ADD_ARROW:
				tool.drawArrowTool(_rendertexture, position,
					_squaresize, y - _numtoolrows);
				return;
			case Tool_State::SET_SQUARE_COLOR:
				tool.drawSetSquareColorTool(_rendertexture, position,
					_squaresize, y - _numtoolrows);
				return;
			case Tool_State::ADD_PROFILE_BOX:
				tool.drawAddProfileBoxTool(_rendertexture, position,
					_squaresize, y - _numtoolrows);
				return;
			case Tool_State::ADD_PIECE_ACCESSORY:
				tool.drawAccessoryTool(_rendertexture, position,
					_squaresize, y - _numtoolrows, tool.getAccessoryName());
				return;
			case Tool_State::ADD_PIECE_CHAR_ACCESSORY:
				tool.drawAccessoryTool(_rendertexture, position,
					_squaresize, y - _numtoolrows, string(1, tool.getCharAccessory()));
				return;
			default:
				Piece piece = getPieceFromId(tool, x, y);
				piece.setSize(_squaresize);
				piece.draw(_rendertexture, position);
				return;
			}
		}
		else
		{
			Piece piece = getPieceFromId(tool, x, y);
			if (piece.exists())
			{
				piece.setSize(_squaresize);
				piece.draw(_rendertexture, position);
			}
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
					(float)_squaresize.x,
					(float)_squaresize.y }
	};
	square.setFillColor(getSquareColor(x, y));
	square.setPosition(sf::Vector2f{
			(float)(_squaresize.x*x),
			(float)(_squaresize.y*y)
		}
	);
	return square;
}