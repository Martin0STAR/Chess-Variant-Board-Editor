#include "Tool.h"

using namespace std;

Tool::Tool()
	:Tool::Tool{ sf::Vector2u{1,1}, Tool_State::SELECT,
	sf::Color{239, 238, 210, 255}, sf::Color(50, 46, 43, 255) }
{}

Tool::Tool(sf::Vector2u size,
	Tool_State::Tool_State state, sf::Color squarecolor,
	sf::Color boardcolor)
{
	_size = size;
	_rendertexture.create(_size.x, _size.y);
	_piecebrush.setStyle("bulldog");
	_piecebrush.setColor(_colortool.getPieceColor());
	_piecebrush.setName("pawn");
	_squarecolor = squarecolor;
	_boardcolor = boardcolor;
	setState(state);
	_accessoryname = "star6point";
	_characcessory = 'J';
}

Tool::Tool(const Tool & rhs)
{
	_size = rhs._size;
	_state = rhs._state;
	_piecebrush = rhs._piecebrush;
	_colortool = rhs._colortool;
	_rendertexture.create(_size.x, _size.y);
	_squarecolor = rhs._squarecolor;
	_boardcolor = rhs._boardcolor;
	_accessoryname = rhs._accessoryname;
	_characcessory = rhs._characcessory;
	_playername = rhs._playername;
}

Tool &Tool::operator =(const Tool & rhs)
{
	_size = rhs._size;
	_state = rhs._state;
	_piecebrush = rhs._piecebrush;
	_colortool = rhs._colortool;
	_rendertexture.create(_size.x, _size.y);
	_squarecolor = rhs._squarecolor;
	_boardcolor = rhs._boardcolor;
	_accessoryname = rhs._accessoryname;
	_characcessory = rhs._characcessory;
	_playername = rhs._playername;
	return *this;
}

void Tool::load(sf::Vector2u size,
	Tool_State::Tool_State state, sf::Color squarecolor,
	sf::Color boardcolor)
{
	_size = size;
	_rendertexture.create(_size.x, _size.y);
	_piecebrush.setStyle("bulldog");
	_piecebrush.setColor(_colortool.getPieceColor());
	_piecebrush.setName("pawn");
	_squarecolor = squarecolor;
	_boardcolor = boardcolor;
	setState(state);
	_accessoryname = "star6point";
}

sf::Color Tool::getBackgroundColor() const
{
	return _squarecolor;
}

Piece& Tool::getPieceBrush()
{
	return _piecebrush;
}

unsigned int Tool::getNumColors()
{
	return _colortool.getSize();
}

unsigned int Tool::getColorIndex()
{
	return _colortool.getIndex();
}

sf::Color Tool::getSquareColor() const
{
	return _colortool.getSquareColor();
}

sf::Color Tool::getArrowColor() const
{
	return _colortool.getArrowColor();
}

sf::Color Tool::getArrowColor(unsigned int index) const
{
	return _colortool.getArrowColor(index);
}

sf::Color Tool::getBorderColor() const
{
	return _colortool.getBorderColor();
}

PieceColor Tool::getPieceColor(unsigned int index) const
{
	return _colortool.getPieceColor(index);
}

Tool_State::Tool_State Tool::getState() const
{
	return _state;
}

string Tool::getAccessoryName() const
{
	return _accessoryname;
}

char Tool::getCharAccessory() const
{
	return _characcessory;
}

sf::Color Tool::getAccessoryColor() const
{
	return _colortool.getAccessoryColor();
}

sf::Color Tool::getAccessoryColor(unsigned int index) const
{
	return _colortool.getAccessoryColor(index);
}

string Tool::getPlayerName() const
{
	return _playername;
}

void Tool::setSquareColor(sf::Color color)
{
	_squarecolor = color;
}

void Tool::setState(Tool_State::Tool_State state)
{
	if (state == Tool_State::ADD_PIECE && !(_piecebrush.exists()))
	{
		_state = Tool_State::REMOVE;
	}
	else
	{
		_state = state;
	}
	updateImage();
}

void Tool::setColorIndex(unsigned int index)
{
	_colortool.setIndex(index);
	_piecebrush.setColor(_colortool.getPieceColor());
	updateImage();
}

void Tool::setPosition(sf::Vector2f position)
{
	_position = position;
}

void Tool::setAccessoryName(string name)
{
	_accessoryname = name;
}

void Tool::setCharAccessory(char character)
{
	_characcessory = character;
}

void Tool::setPlayerName(string name)
{
	_playername = name;
}

void Tool::flipPieceHorizontally()
{
	_piecebrush.flipHorizontally();
}

void Tool::flipPieceVertically()
{
	_piecebrush.flipVertically();
}

void Tool::invertPieceColor()
{
	_piecebrush.invertColors();
}

void Tool::updateImage()
{
	switch (_state)
	{
	case Tool_State::ADD_PIECE:
	{
		_rendertexture.clear(_squarecolor);
		_piecebrush.setWidth(_size.x);
		_piecebrush.setHeight(_size.y);
		_piecebrush.draw(_rendertexture, sf::Vector2f{ 0.f, 0.f });
		break;
	}
	case Tool_State::SET_SQUARE_COLOR:
		_rendertexture.clear(_squarecolor);
		drawSetSquareColorTool(_rendertexture, sf::Vector2f{ 0.f, 0.f },
			_size, getColorIndex());
		break;
	case Tool_State::ADD_ARROW:
	{
		_rendertexture.clear(_squarecolor);
		drawArrowTool(_rendertexture, sf::Vector2f{ 0.f, 0.f },
			_size, getColorIndex());
		break;
	}
	case Tool_State::ADD_PROFILE_BOX:
		_rendertexture.clear(_boardcolor);
		drawAddProfileBoxTool(_rendertexture, sf::Vector2f{ 0.f, 0.f },
			_size, getColorIndex());
		break;
	case Tool_State::REMOVE_SQUARE:
		_rendertexture.clear(_boardcolor);
		break;
	case Tool_State::ADD_PIECE_ACCESSORY:
		_rendertexture.clear(_squarecolor);
		drawAccessoryTool(_rendertexture, sf::Vector2f{ 0.f, 0.f },
			_size, getColorIndex(), getAccessoryName());
		break;
	case Tool_State::ADD_PIECE_CHAR_ACCESSORY:
		_rendertexture.clear(_squarecolor);
		drawAccessoryTool(_rendertexture, sf::Vector2f{ 0.f, 0.f },
			_size, getColorIndex(), string(1,getCharAccessory()));
		break;
	default:
		_rendertexture.clear(_squarecolor);
		string filename = getFileName();
		if (std::experimental::filesystem::exists(filename))
		{
			sf::Image icon;
			icon.loadFromFile(filename);
			sf::Texture texture;
			texture.loadFromImage(icon);
			sf::Sprite sprite{ texture };
			_rendertexture.draw(sprite);
		}
		else
		{
			sf::Image icon;
			icon.loadFromFile(getFileName(Tool_State::SELECT));
			sf::Texture texture;
			texture.loadFromImage(icon);
			sf::Sprite sprite{ texture };
			_rendertexture.draw(sprite);
		}
		break;
	}
	_rendertexture.display();
}

void Tool::drawRemoveSquareTool(
	sf::RenderTarget & target, sf::Vector2f position,
	sf::Vector2u size)
{
	sf::RectangleShape shape{ sf::Vector2f{(float)size.x, (float)size.y} };
	shape.setFillColor(_boardcolor);
	shape.setPosition(position);
	target.draw(shape);
}

void Tool::drawSetSquareColorTool(
	sf::RenderTarget & target, sf::Vector2f position,
	sf::Vector2u size,
	unsigned int colorindex)
{
	sf::RectangleShape shape{ sf::Vector2f{(float)size.x, (float)size.y} };
	shape.setFillColor(_colortool.getSquareColor(colorindex));
	shape.setPosition(position);
	target.draw(shape);
}

void Tool::drawArrowTool(
	sf::RenderTarget & target, sf::Vector2f position,
	sf::Vector2u size,
	unsigned int colorindex)
{
	Arrow arrow{
		position + sf::Vector2f{(float)size.x / 2.f, (float)size.y},
		position + sf::Vector2f{(float)size.x / 2.f, 0.f},
		getArrowColor(colorindex)
	};
	arrow.draw(target);
}

void Tool::drawAddProfileBoxTool(
	sf::RenderTarget & target, sf::Vector2f position,
	sf::Vector2u size,
	unsigned int colorindex)
{
	sf::RectangleShape shape{ sf::Vector2f{(float)size.x, (float)size.y} };
	shape.setFillColor(_boardcolor);
	shape.setPosition(position);
	target.draw(shape);

	BoardComponent::ProfileBox profilebox{
		_playername,
		position + sf::Vector2f{(float)size.x / 2.f, (float)size.y / 2.f},
		size, 3, 3,
		_colortool.getBorderColor(colorindex),
		sf::Color::Transparent };
	profilebox.draw(target);
}

void Tool::drawAccessoryTool(
	sf::RenderTarget & target, sf::Vector2f position,
	sf::Vector2u size,
	unsigned int colorindex, string toolname)
{
	PieceAccessory a{ toolname, getAccessoryColor(colorindex) };
	a.setPosition(
		position + sf::Vector2f{
			(float)size.x / 8, (float)size.y / 10 },
			sf::Vector2u{ size.x * 3 / 4, size.y * 4 / 5 });
	a.draw(target);
}

void Tool::draw(sf::RenderTarget & target)
{
	sf::Sprite sprite{ _rendertexture.getTexture() };
	sprite.setPosition(_position);
	target.draw(sprite);
}

string Tool::getName()
{
	return getName(_state);
}

string Tool::getName(Tool_State::Tool_State state)
{
	auto search = _toolnames.find(state);
	if (search != _toolnames.end())
	{
		return search->second;
	}
	return "";
}

string Tool::getFileName()
{
	return getFileName(_state);
}

string Tool::getFileName(Tool_State::Tool_State state)
{
	return "resources/icons/" + getName(state) + ".png";
}