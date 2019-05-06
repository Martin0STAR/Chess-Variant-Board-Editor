#include "Board.h"

using namespace std;

Board::Board()
	:_displaysize{ sf::Vector2u{ 500, 500 } },
	_textsize{ 32 },
	_textcolor{ sf::Color(155, 155, 155, 255) },
	_displaytextcolor{ sf::Color::Black },
	_textbackgroundcolor{ sf::Color::White },
	_textbackgroundleftoffset{ 5 },
	_textbackgroundtopoffset{ 5 }
{}//needs to run load(name)

Board::Board(string name)
	: Board::Board{}
{
	load(name);
}

bool Board::load(string name)
{
	return intern_load(getImageFileName(name), getSetupFileName(name));
}

bool Board::initLoad(string name)
{
	return intern_load(getInitImageFileName(name), getInitSetupFileName(name));
}

Board::~Board()
{
	_piece_list.clear();
	_colored_square_list.clear();
	_highlighted_square_list.clear();
	_arrow_list.clear();
	_removed_square_list.clear();
	_profile_box_list.clear();
	_squarecolors.clear();
}

sf::Vector2u Board::getImageSize() const
{
	return _rendertexture.getSize();
}

sf::Vector2u Board::getDisplaySize() const
{
	return _displaysize;
}

sf::Vector2u Board::getImageSquareSize() const
{
	return _squaresize;
}

sf::Vector2f Board::getDisplaySquareSize() const
{
	return sf::Vector2f{
		(float)(_squaresize.x*_displaysize.x) / (float)_rendertexture.getSize().x,
		(float)(_squaresize.y*_displaysize.y) / (float)_rendertexture.getSize().y
	};
}

sf::Vector2u Board:: getImageSquareOffset() const
{
	return sf::Vector2u{ _leftofboardwidth, _topofboardheight };
}

sf::Vector2f Board::getDisplaySquareOffset() const
{
	return sf::Vector2f{
		(float)(_leftofboardwidth * _displaysize.x) / (float)_rendertexture.getSize().x,
		(float)(_topofboardheight * _displaysize.y) / (float)_rendertexture.getSize().y
	};
}

sf::Vector2i Board::getImageCoordFromDisplayCoord(sf::Vector2f displaypos) const
{
	return sf::Vector2i{
		(int)(displaypos.x * _rendertexture.getSize().x / _displaysize.x),
		(int)(displaypos.y * _rendertexture.getSize().y / _displaysize.y)
	};
}

unsigned int Board::getNumRows() const
{
	return _numrows;
}

unsigned int Board::getNumColumns() const
{
	return _numcolumns;
}

sf::Color Board::getBoardColor() const
{
	return _boardcolor;
}

sf::Color Board::getSquareColor(unsigned int index) const
{
	if (index < _squarecolors.size())
	{
		return _squarecolors.at(index);
	}
	return sf::Color::Transparent;
}

sf::Color Board::getSquareColor(BoardComponent::Coord coord) const
{
	auto it = _colored_square_list.find(coord);
	if (it != _colored_square_list.cend())
	{
		return it->second;
	}

	return _squarecolors.at(coord.getColorId(_squarecolors.size()));
}

BoardComponent::Square Board::getSquare(BoardComponent::Coord coord) const
{
	return BoardComponent::Square{
		getSquareColor(coord),
		_squaresize,
		_squarecornersize,
		isTopLeft(coord),
		isTopRight(coord),
		isBottomLeft(coord),
		isBottomRight(coord)
	};
}

Piece Board::getPiece(BoardComponent::Coord coord) const
{
	return _piece_list.at(coord);
}

std::vector<BoardComponent::ProfileBox> Board::getProfileBoxList() const
{
	return _profile_box_list;
}

bool Board::isWithinPixelBoard(sf::Vector2f pixelpos) const
{
	return pixelpos.x >= (float)_leftofboardwidth &&
		pixelpos.x < (float)_rendertexture.getSize().x - (float)_rightofboardwidth &&
		pixelpos.y >= (float)_topofboardheight &&
		pixelpos.y < (float)_rendertexture.getSize().y - (float)_bottomofboardheight;
}

bool Board::isWithinBoard(sf::Vector2i coord) const
{
	return !(coord.x < 0 ||
		coord.x >= (int)_numcolumns ||
		coord.y < 0 ||
		coord.y >= (int)_numrows);
}

bool Board::isWithinBoard(BoardComponent::Coord coord) const
{
	return isWithinBoard(coord.getVector());
}

bool Board::isValidSquare(BoardComponent::Coord coord) const
{
	auto it = find(_removed_square_list.begin(), _removed_square_list.end(), coord);
	if (it != _removed_square_list.end())
	{
		return false;
	}
	return isWithinBoard(coord);
}

bool Board::isEmptySquare(BoardComponent::Coord coord) const
{
	return _piece_list.find(coord) == _piece_list.end();
}

bool Board::togglegrid()
{
	_showgrid = !_showgrid;
	return true;
}

void Board::removeDragArrow()
{
	_showdragarrow = false;
}

void Board::setText(std::string text)
{
	_text.setString(text);

	sf::FloatRect textrect = _text.getLocalBounds();
	_text.setOrigin(textrect.left + textrect.width / 2.f,
		textrect.top + textrect.height / 2.f);

	_textbackground.setSize(sf::Vector2f{
		textrect.width + 2 * _textbackgroundleftoffset,
		textrect.height + 2 * _textbackgroundtopoffset });
	_textbackground.setOrigin(
		_textbackground.getSize().x / 2.f,
		_textbackground.getSize().y / 2.f);
	_textbackground.setPosition(_text.getPosition());
}

void Board::showText()
{
	_showtext = true;
}

void Board::hideText()
{
	_showtext = false;
}

bool Board::addRowUp()
{
	if (addRowDown())
	{
		_squarecolors.insert(_squarecolors.begin(), _squarecolors.back());
		_squarecolors.pop_back();
		moveAllDown();
		return true;
	}
	return false;
}

bool Board::addRowDown()
{
	_numrows++;
	_rendertexture.create(
		_rendertexture.getSize().x,
		_rendertexture.getSize().y + _squaresize.y);
	return true;
}

bool Board::addColumnLeft()
{
	if (addColumnRight())
	{
		_squarecolors.insert(_squarecolors.begin(), _squarecolors.back());
		_squarecolors.pop_back();
		moveAllRight();
		return true;
	}
	return false;
}

bool Board::addColumnRight()
{
	_numcolumns++;
	_rendertexture.create(
		_rendertexture.getSize().x + _squaresize.x,
		_rendertexture.getSize().y);
	return true;
}


bool Board::removeRowUp()
{
	if (removeRowDown())
	{
		_squarecolors.insert(_squarecolors.begin(), _squarecolors.back());
		_squarecolors.pop_back();
		moveAllUp();
		return true;
	}
	return false;
}

bool Board::removeRowDown()
{
	if (_numrows < 2)
	{
		return false;
	}
	_numrows--;
	_rendertexture.create(
		_rendertexture.getSize().x,
		_rendertexture.getSize().y - _squaresize.y);
	return true;
}

bool Board::removeColumnLeft()
{
	if (removeColumnRight())
	{
		_squarecolors.insert(_squarecolors.begin(), _squarecolors.back());
		_squarecolors.pop_back();
		moveAllLeft();
		return true;
	}
	return false;
}

bool Board::removeColumnRight()
{
	if (_numcolumns < 2)
	{
		return false;
	}
	_numcolumns--;
	_rendertexture.create(
		_rendertexture.getSize().x - _squaresize.x,
		_rendertexture.getSize().y);
	return true;
}

bool Board::setSquareColor(BoardComponent::Coord coord, sf::Color color)
{
	auto it = _colored_square_list.find(coord);
	if (it == _colored_square_list.end())
	{
		_colored_square_list.insert({ coord, color });
	}
	else if (it->second == color)
	{
		_colored_square_list.erase(it);
	}
	else
	{
		_colored_square_list.erase(it);
		_colored_square_list.insert({ coord, color });
	}
	return true;
}

bool Board::removeSquare(BoardComponent::Coord coord)
{
	auto it = find(_removed_square_list.begin(), _removed_square_list.end(), coord);

	if (it == _removed_square_list.end())
	{
		_removed_square_list.push_back(coord);
	}
	else
	{
		_removed_square_list.erase(it);
	}
	return true;
}

bool Board::addHighlightSquare(BoardComponent::Coord coord, sf::Color color)
{
	auto it = _highlighted_square_list.find(coord);
	if (it == _highlighted_square_list.end())
	{
		_highlighted_square_list.insert({ coord, color });
	}
	else if (it->second == color)
	{
		removeHighlightSquare(coord);
	}
	else
	{
		removeHighlightSquare(coord);
		_highlighted_square_list.insert({ coord, color });
	}
	return true;
}

bool Board::removeHighlightSquare(BoardComponent::Coord coord)
{
	auto it = _highlighted_square_list.find(coord);
	if (it != _highlighted_square_list.end())
	{
		_highlighted_square_list.erase(it);
		return true;
	}
	return false;
}



bool Board::addPiece(Piece & piece, BoardComponent::Coord coord)
{
	auto it = _piece_list.find(coord);
	if (it == _piece_list.end())
	{
		_piece_list.insert({ coord, piece });
	}
	else if (it->second == piece)
	{
		removePiece(coord);
	}
	else if (it->second.isCarrier() && !(piece.isCarrier()))
	{
		addPieceOnTop(piece, coord);
	}
	else
	{
		removePiece(coord);
		_piece_list.insert({ coord, piece });
	}
	return true;
}

bool Board::removePiece(BoardComponent::Coord coord)
{
	auto it = _piece_list.find(coord);
	if (it != _piece_list.end())
	{
		_piece_list.erase(it);
		return true;
	}
	return false;
}


bool Board::addPieceOnTop(Piece& pieceontop, BoardComponent::Coord coord)
{
	if (isEmptySquare(coord))
	{
		return false;
	}
	Piece& piece = _piece_list.at(coord);
	piece.addPieceOnTop(pieceontop);
	return true;
}

bool Board::removePieceOnTop(BoardComponent::Coord coord)
{
	if (isEmptySquare(coord))
	{
		return false;
	}
	return _piece_list.at(coord).removePieceOnTop();
}

bool Board::addPieceAccessory(
	BoardComponent::Coord coord, std::string accessoryname, sf::Color color)
{
	return _piece_list.at(coord).addAccessory(accessoryname, color);
}


bool Board::addArrow(BoardComponent::Coord fromcoord, BoardComponent::Coord tocoord, sf::Color color)
{
	sf::Vector2f frompos{	getCenterPosition(fromcoord)};
	sf::Vector2f topos{ getCenterPosition(tocoord)};

	Arrow newarrow{ frompos, topos, color };

	auto it = find_if(_arrow_list.begin(), _arrow_list.end(), [newarrow](Arrow oldarrow)
	{
		return oldarrow.getFromCoord() == newarrow.getFromCoord() &&
			oldarrow.getToCoord() == newarrow.getToCoord();
	});
	
	if (it == _arrow_list.end())
	{
		_arrow_list.push_back(newarrow);
	}
	else if(*it == newarrow)
	{
		removeArrow(newarrow);
	}
	else
	{
		removeArrow(*it);
		_arrow_list.push_back(newarrow);
	}
	return true;
}

bool Board::removeArrow(Arrow & arrow)
{
	_arrow_list.erase(remove(_arrow_list.begin(), _arrow_list.end(), arrow), _arrow_list.end());
	return true;
}



bool Board::addProfileBox(BoardComponent::Coord coord, string name, sf::Color bordercolor)
{

	sf::Vector2f position{ getCenterPosition(coord) };
	
	BoardComponent::ProfileBox profilebox{ name,  position, _profilesize, _profileborder1width, _profileborder2width, bordercolor, _profilehighlightcolor };

	if (_profile_box_list.size() == 0)
	{
		profilebox.highlight();
	}
	_profile_box_list.push_back(profilebox);
	return true;
}



bool Board::removeProfileBox(unsigned int playerid)
{
	if (playerid >= _profile_box_list.size())
	{
		return false;
	}
	_profile_box_list.erase(_profile_box_list.begin() + playerid);
	return true;
}

bool Board::highlightPlayerBox(unsigned int playerid)
{
	if (playerid >= _profile_box_list.size())
	{
		return false;
	}
	if (_profile_box_list.at(playerid).highlight())
	{
		for (unsigned int i{ 0 }; i < _profile_box_list.size(); i++)
		{
			if (i != playerid)
			{
				_profile_box_list.at(i).unhighlight();
			}
		}
		return true;
	}
	return false;
}

bool Board::movePiece(BoardComponent::Coord fromcoord, BoardComponent::Coord tocoord)
{
	if (fromcoord == tocoord)
	{
		return false;
	}
	auto it = _piece_list.find(fromcoord);
	if (it == _piece_list.end())
	{
		return false;
	}
	removePiece(tocoord);
	addPiece(it->second, tocoord);
	removePiece(fromcoord);
	return true;
}

bool Board::moveAll(BoardComponent::Coord movecoord)
{
	std::map<BoardComponent::Coord, Piece> newpiecelist;
	for (auto it : _piece_list)
	{
		newpiecelist.insert({ it.first + movecoord, it.second });
	}
	_piece_list.clear();
	_piece_list = newpiecelist;

	std::map<BoardComponent::Coord, sf::Color> newcoloredsquarelist;
	for (auto it : _colored_square_list)
	{
		newcoloredsquarelist.insert({ it.first + movecoord, it.second });
	}
	_colored_square_list.clear();
	_colored_square_list = newcoloredsquarelist;

	std::map<BoardComponent::Coord, sf::Color> newhighlightedsquarelist;
	for (auto it : _highlighted_square_list)
	{
		newhighlightedsquarelist.insert({ it.first + movecoord, it.second });
	}
	_highlighted_square_list.clear();
	_highlighted_square_list = newhighlightedsquarelist;

	sf::Vector2f pixelvector{
		(float)(movecoord.getVector().x * (float)_squaresize.x),
		(float)(movecoord.getVector().y * (float)_squaresize.y)
	};

	for (auto & arrow : _arrow_list)
	{
		arrow.move(pixelvector);
	}

	for (auto & removedsquare : _removed_square_list)
	{
		removedsquare = removedsquare + movecoord;
	}

	for (auto & profilebox : _profile_box_list)
	{
		profilebox.move(pixelvector);
	}
	return true;
}

bool Board::moveAllUp()
{
	BoardComponent::Coord movecoord{ sf::Vector2i{0, -1 } };
	return moveAll(movecoord);
}

bool Board::moveAllDown()
{
	BoardComponent::Coord movecoord{ sf::Vector2i{0, 1} };
	return moveAll(movecoord);
}

bool Board::moveAllLeft()
{
	BoardComponent::Coord movecoord{ sf::Vector2i{-1, 0} };
	return moveAll(movecoord);
}

bool Board::moveAllRight()
{
	BoardComponent::Coord movecoord{ sf::Vector2i{1, 0} };
	return moveAll(movecoord);
}

bool Board::generateColoredBoard(std::default_random_engine & rng)
{
	_colored_square_list.clear();

	array<sf::Color, 6> colorlist
	{
		sf::Color{164,63,51,255},
		sf::Color{65,64,142,255},
		sf::Color{216,214,33,255},
		sf::Color{91,145,61,255},
		sf::Color{83,147,191,255},
		sf::Color{224,144,45,255}
	};
	shuffle(colorlist.begin(), colorlist.end(), rng);

	vector<sf::Color> squarecolorlist1;
	vector<sf::Color> squarecolorlist2;
	int numsquares = _numrows * _numcolumns;
	for (int i{ 0 }; i < numsquares; i++)
	{
		if (i < numsquares / 2)
		{
			squarecolorlist1.push_back(colorlist.at(i % colorlist.size()));
		}
		else
		{
			squarecolorlist2.push_back(colorlist.at(i % colorlist.size()));
		}
	}
	shuffle(squarecolorlist1.begin(), squarecolorlist1.end(), rng);
	shuffle(squarecolorlist2.begin(), squarecolorlist2.end(), rng);

	for (unsigned int y{ 0 }; y < _numrows; y++)
	{
		for (unsigned int x{ 0 }; x < _numcolumns; x++)
		{
			BoardComponent::Coord coord{ sf::Vector2i{(int)x, (int)y} };
			if (squarecolorlist1.size() > 0)
			{
				sf::Color color = squarecolorlist1.back();
				setSquareColor(coord, color);
				squarecolorlist1.pop_back();
			}
			else
			{
				sf::Color color = squarecolorlist2.back();
				setSquareColor(coord, color);
				squarecolorlist2.pop_back();
			}
		}
	}
	return true;
}

void Board::updateImage()
{
	_rendertexture.clear(sf::Color::Transparent);
	drawEmptyBoard();

	for (unsigned int y{ 0 }; y < _numrows; y++)
	{
		for (unsigned int x{ 0 }; x < _numcolumns; x++)
		{
			BoardComponent::Coord coord{ sf::Vector2i{(int)x, (int)y} };
			drawEmptySquare(coord);
		}
	}

	for (auto highlighted_square : _highlighted_square_list)
	{
		drawColoredSquare(highlighted_square.second, highlighted_square.first);
	}

	if (_showgrid)
	{
		drawGrid();
	}

	drawCoordinates();

	for (auto & piece : _piece_list)
	{
		drawPiece(piece.second, piece.first);
	}

	for (auto & arrow : _arrow_list)
	{
		drawArrow(arrow);
	}

	for (auto & profilebox : _profile_box_list)
	{
		drawProfileBox(profilebox);
	}

	_rendertexture.display();
}

void Board::updateDragArrow(BoardComponent::Coord fromcoord, BoardComponent::Coord tocoord)
{
	if (!isValidSquare(fromcoord) ||
		!isValidSquare(tocoord) ||
		fromcoord == tocoord)
	{
		_showdragarrow = false;
		return;
	}

	_dragarrow.setPosition(getDisplayCenterPosition(fromcoord), getDisplayCenterPosition(tocoord));
	_showdragarrow = true;
}

bool Board::updateProfileBox(unsigned int id, std::string name, sf::Color bordercolor)
{
	if (id >= _profile_box_list.size())
	{
		return false;
	}
	return _profile_box_list.at(id).set(name, bordercolor);
}



bool Board::drawEmptyBoard()
{
	BoardComponent::Square board{
		_boardcolor,
		getImageSize(),
		_boardcornersize,
		true,
		true,
		true,
		true
	};

	board.draw(_rendertexture, sf::Vector2f{ 0.f, 0.f });
	return true;
}

bool Board::drawEmptySquare(BoardComponent::Coord coord)
{
	if (!isValidSquare(coord))
	{
		return false;
	}

	BoardComponent::Square square{
		getSquareColor(coord),
		_squaresize,
		_squarecornersize,
		isTopLeft(coord),
		isTopRight(coord),
		isBottomLeft(coord),
		isBottomRight(coord)
	};
	square.draw(_rendertexture, getPosition(coord));

	return true;
}

bool Board::drawGrid()
{
	sf::RectangleShape verticalline{
		sf::Vector2f{2.f, (float)(_squaresize.y*_numrows)} };
	verticalline.setFillColor(_boardcolor);
	for (unsigned int i{ 1 }; i < _numcolumns; i++)
	{
		verticalline.setPosition(
			(float)(_leftofboardwidth + _squaresize.x * i - 1),
			(float)_topofboardheight);
		_rendertexture.draw(verticalline);
	}

	sf::RectangleShape horizontalline{
		sf::Vector2f{(float)(_squaresize.x*_numcolumns), 2.f} };
	horizontalline.setFillColor(_boardcolor);
	for (unsigned int i{ 1 }; i < _numrows; i++)
	{
		horizontalline.setPosition(
			(float)_leftofboardwidth,
			(float)(_topofboardheight + _squaresize.y * i - 1));
		_rendertexture.draw(horizontalline);
	}
	return true;
}

bool Board::drawPiece(Piece & piece, BoardComponent::Coord coord)
{
	if (!isValidSquare(coord))
	{
		return false;
	}
	piece.setWidth(_squaresize.x);
	piece.setHeight(_squaresize.y);
	piece.draw(_rendertexture, getPosition(coord));

	return true;
}

bool Board::drawArrow(Arrow & arrow)
{
	if (!isWithinPixelBoard(arrow.getFromCoord()) ||
		!isWithinPixelBoard(arrow.getToCoord()))
	{
		return false;
	}
	arrow.draw(_rendertexture);
	return true;
}

bool Board::drawColoredSquare(sf::Color color, BoardComponent::Coord coord)
{
	if (!isValidSquare(coord))
	{
		return false;
	}

	BoardComponent::Square square{
		color,
		_squaresize,
		_squarecornersize,
		isTopLeft(coord),
		isTopRight(coord),
		isBottomLeft(coord),
		isBottomRight(coord)
	};
	square.draw(_rendertexture, getPosition(coord));

	return true;
}

bool Board::drawProfileBox(BoardComponent::ProfileBox profilebox)
{
	if (!isWithinPixelBoard(profilebox.getTopLeftPosition()) ||
		!isWithinPixelBoard(profilebox.getBottomRightPosition()))
	{
		return false;
	}
	profilebox.draw(_rendertexture);
	return true;
}

void Board::draw(sf::RenderTarget & target)
{
	sf::Sprite sprite{ _rendertexture.getTexture() };
	sprite.setScale(
		((float)_displaysize.x) / _rendertexture.getSize().x,
		((float)_displaysize.y) / _rendertexture.getSize().y
	);
	target.draw(sprite);

	if (_showdragarrow)
	{
		_dragarrow.draw(target);
	}

	if (_showtext)
	{
		target.draw(_textbackground);
		target.draw(_text);
	}
}

void Board::save(string name)
{
	_rendertexture.getTexture().copyToImage().saveToFile(getImageFileName(name));

	ofstream ofs;
	ofs.open(getSetupFileName(name));
	ofs << "[Rows " << _numrows << "]\n"
		<< "[Columns " << _numcolumns << "]\n"
		<< "[SquareWidth " << _squaresize.x << "]\n"
		<< "[SquareHeight " << _squaresize.y << "]\n"
		<< "[BoardCornerSize " << _boardcornersize << "]\n"
		<< "[SquareCornerSize " << _squarecornersize << "]\n"
		<< "[LeftOffset " << _leftofboardwidth << "]\n"
		<< "[TopOffset " << _topofboardheight << "]\n"
		<< "[BoardColor " << _boardcolor.toInteger() << "]\n"
		<< "[SquareColors";

	for (auto elem : _squarecolors)
	{
		ofs << " " << elem.toInteger();
	}
	ofs << "]\n";

	ofs << "[NotationSize " << _notationsize << "]\n";

	if (_showgrid)
	{
		ofs << "[Grid_On]\n";
	}

	for (auto elem : _piece_list)
	{
		ofs << "[Piece " << elem.second.getName()
			<< " " << elem.second.getColor().name
			<< " " << elem.first.getNotation(getNumRows())
			<< " -" << elem.second.getFlags();
		if (elem.second.colorIsModified())
		{
			ofs << " " << elem.second.getColor().adder.toInteger()
				<< " " << elem.second.getColor().subtracter.toInteger()
				<< " " << elem.second.getColor().multiplier.toInteger();
		}
		if (elem.second.hasAccessories())
		{
			ofs << " [";
			auto alist = elem.second.getAccessorylist();
			for (auto it = alist.begin();
				it != alist.end();
				it++)
			{
				ofs << it->getName() << " " << it->getColor().toInteger();
				if (it + 1 != alist.end())
				{
					ofs << " ";
				}
			}
			ofs << "]";
		}
		if (elem.second.isCarrying())
		{
			const Piece & pieceontop = elem.second.getPieceOnTop();
			ofs << " [" << pieceontop.getName()
				<< " " << pieceontop.getColor().name
				<< " -" << pieceontop.getFlags();
			if (pieceontop.colorIsModified())
			{
				ofs << " " << pieceontop.getColor().adder.toInteger()
					<< " " << pieceontop.getColor().subtracter.toInteger()
					<< " " << pieceontop.getColor().multiplier.toInteger();
			}
			ofs << "]";
		}
		ofs << "]\n";
	}

	for (auto elem : _colored_square_list)
	{
		ofs << "[Colored_Square " << elem.second.toInteger()
			<< " " << elem.first.getNotation(getNumRows()) << "]\n";
	}

	for (auto elem : _highlighted_square_list)
	{
		ofs << "[Highlighted_Square " << elem.second.toInteger()
			<< " " << elem.first.getNotation(getNumRows()) << "]\n";
	}

	for (auto arrow : _arrow_list)
	{
		ofs << "[Arrow "
			<< arrow.getFromCoord().x << " "
			<< arrow.getFromCoord().y << " "
			<< arrow.getToCoord().x << " "
			<< arrow.getToCoord().y << " "
			<< arrow.getColor().toInteger() << "]\n";
	}

	if (_removed_square_list.size() != 0)
	{
		sort(_removed_square_list.begin(), _removed_square_list.end());
		ofs << "[Removed_Squares ";

		for (auto it = _removed_square_list.cbegin(); it != _removed_square_list.cend(); it++)
		{
			ofs << it->getNotation(_numrows);
			if (it + 1 != _removed_square_list.cend())
			{
				ofs << " ";
			}
		}
		ofs << "]\n";
	}

	for (auto profile : _profile_box_list)
	{
		profile.saveProfileImage();
		ofs << "[Profilebox "
			<< profile.getName() << " "
			<< profile.getPosition().x << " "
			<< profile.getPosition().y << " "
			<< profile.getSize().x << " "
			<< profile.getSize().y << " "
			<< profile.getBorderWidth() << " "
			<< profile.getHighlightWidth() << " "
			<< profile.getPlayerColor().toInteger() << " "
			<< profile.getHighlightColor().toInteger() << " ";
		if (profile.isHighlighted())
		{
			ofs << "1";
		}
		else
		{
			ofs << "0";
		}
		ofs << "]\n";
	}
}

void Board::newNameSave(string newname, string oldname)
{
	save(newname);
	std::remove(("savedboards\\" + oldname + ".png").c_str());
	std::remove(("savedboards/setup/" + oldname + ".txt").c_str());
}

bool Board::intern_load(string imagefilename, string textfilename)
{
	if (!experimental::filesystem::exists(imagefilename) ||
		!experimental::filesystem::exists(textfilename))
	{
		return false;
	}
	_showgrid = false;
	_showdragarrow = false;
	_showtext = false;

	_profilesize = sf::Vector2u{ 110, 110 };
	_profileborder1width = 5;
	_profileborder2width = 5;
	_profilehighlightcolor = sf::Color(255, 255, 255, 255);

	_textbelowboardoffset = 3;

	_squarecolors.clear();
	_removed_square_list.clear();
	_colored_square_list.clear();
	_highlighted_square_list.clear();
	_piece_list.clear();
	_arrow_list.clear();
	_profile_box_list.clear();
	{
		sf::Image image;
		image.loadFromFile(imagefilename);
		_rendertexture.create(image.getSize().x, image.getSize().y);
		sf::Texture texture;
		texture.loadFromImage(image);
		sf::Sprite sprite{ texture };
		_rendertexture.clear();
		
		_rendertexture.draw(sprite, sf::BlendNone);
		_rendertexture.display();
	}

	string filename = getFontFileName();
	if (_font.loadFromFile(filename))
	{
		_text.setFont(_font);
		_text.setCharacterSize(_textsize);
		_text.setFillColor(_displaytextcolor);
		_text.setString("");
		sf::Vector2f position{
		(float)_displaysize.x / 2.f,
		(float)_displaysize.y / 2.f
		};
		_text.setPosition(position);
	}
	else
	{
		cerr << "unable to find font in " << filename << endl;
	}


	ifstream ifs;
	ifs.open(textfilename);
	string variable_name;
	char c;
	while (ifs >> c)
	{
		if (c == '[')
		{
			ifs >> variable_name;
			if (variable_name.back() == ']')
			{
				variable_name.pop_back();
			}
			if (variable_name == "Rows")
			{
				ifs >> _numrows;
			}
			else if (variable_name == "Columns")
			{
				ifs >> _numcolumns;
			}
			else if (variable_name == "SquareWidth")
			{
				ifs >> _squaresize.x;
			}
			else if (variable_name == "SquareHeight")
			{
				ifs >> _squaresize.y;
			}
			else if (variable_name == "BoardCornerSize")
			{
				ifs >> _boardcornersize;
			}
			else if (variable_name == "SquareCornerSize")
			{
				ifs >> _squarecornersize;
			}
			else if (variable_name == "LeftOffset")
			{
				ifs >> _leftofboardwidth;
			}
			else if (variable_name == "TopOffset")
			{
				ifs >> _topofboardheight;
			}
			else if (variable_name == "BoardColor")
			{
				uint32_t intcolor;
				ifs >> intcolor;
				_boardcolor = sf::Color{ intcolor };
			}
			else if (variable_name == "SquareColors")
			{
				uint32_t intcolor;
				ifs >> intcolor;
				_squarecolors.push_back(sf::Color{ intcolor });
				ifs >> intcolor;
				_squarecolors.push_back(sf::Color{ intcolor });
			}
			else if (variable_name == "NotationSize")
			{
				ifs >> _notationsize;
			}
			else if (variable_name == "Piece")
			{
				string piecename;
				PieceColor piececolor;
				string notation;
				string flags;
				ifs >> piecename;
				ifs >> piececolor.name;
				ifs >> notation;
				ifs >> flags;

				Piece piece{ "bulldog", piecename, piececolor, flags };
				if (flags.find('p') != string::npos)
				{
					uint32_t intcolor;
					ifs >> intcolor;
					sf::Color adder{ intcolor };
					ifs >> intcolor;
					sf::Color subtracter{ intcolor };
					ifs >> intcolor;
					sf::Color multiplier{ intcolor };
					piece.setColorAdder(adder);
					piece.setColorSubtracter(subtracter);
					piece.setColorMultiplier(multiplier);
				}
				if (flags.find('a') != string::npos)
				{
					ifs >> ws;
					ifs.ignore(1);
					std::string accessoryname;
					uint32_t intcolor;
					while (ifs.peek() != ']')
					{
						ifs >> accessoryname >> intcolor;
						piece.addAccessory(accessoryname, sf::Color{ intcolor });
						ifs >> ws;
					}
				}
				if (flags.find('c') != string::npos)
				{
					ifs >> ws;
					ifs.ignore(1);
					ifs >> piecename;
					ifs >> piececolor.name;
					ifs >> flags;
					Piece pieceontop{ "bulldog", piecename, piececolor, flags };
					if (flags.find('p') != string::npos)
					{
						uint32_t intcolor;
						ifs >> intcolor;
						sf::Color adder{ intcolor };
						ifs >> intcolor;
						sf::Color subtracter{ intcolor };
						ifs >> intcolor;
						sf::Color multiplier{ intcolor };
						pieceontop.setColorAdder(adder);
						pieceontop.setColorSubtracter(subtracter);
						pieceontop.setColorMultiplier(multiplier);
					}
					piece.addPieceOnTop(pieceontop);
				}
				BoardComponent::Coord coord{ getNumRows(), notation };
				_piece_list.insert({ coord, piece });
			}
			else if (variable_name == "Colored_Square")
			{
				uint32_t intcolor;
				ifs >> intcolor;
				sf::Color color{ intcolor };
				string notation;
				ifs >> notation;
				BoardComponent::Coord coord{ getNumRows(), notation };
				_colored_square_list.insert({ coord, color });
			}
			else if (variable_name == "Highlighted_Square")
			{
				uint32_t intcolor;
				ifs >> intcolor;
				sf::Color color{ intcolor };
				string notation;
				ifs >> notation;
				BoardComponent::Coord coord{ getNumRows(), notation };
				_highlighted_square_list.insert({ coord, color });
			}
			else if (variable_name == "Arrow")
			{
				float fx, fy, tx, ty;
				ifs >> fx >> fy >> tx >> ty;
				uint32_t intcolor;
				ifs >> intcolor;
				sf::Color color{ intcolor };
				Arrow arrow{ sf::Vector2f{fx,fy}, sf::Vector2f{tx,ty}, color };
				_arrow_list.push_back(arrow);
			}
			else if (variable_name == "Removed_Squares")
			{
				string line;
				ifs >> ws;
				getline(ifs, line);
				string notations;
				stringstream ss{ line };
				ifs >> ws;
				vector<string> notationlist;
				while (getline(ss, notations, ']'))
				{
					istringstream iss(notations);
					copy(istream_iterator<string>(iss),
						istream_iterator<string>(),
						back_inserter(notationlist));
					ifs >> ws;
				}
				for (auto notation : notationlist)
				{
					_removed_square_list.push_back(BoardComponent::Coord{ getNumRows(), notation });
				}
				ifs >> ws;
			}
			else if (variable_name == "Profilebox")
			{
				string name;
				sf::Vector2f position;
				sf::Vector2u size;
				unsigned int borderwidth, highlightwidth, ishighlighted;
				uint32_t intplayercolor, inthighlightcolor;
				ifs >> name >> position.x >> position.y >> size.x >> size.y
					>> borderwidth >> highlightwidth
					>> intplayercolor >> inthighlightcolor >> ishighlighted;
				BoardComponent::ProfileBox profilebox{ name, position, size,
					borderwidth, highlightwidth, sf::Color{intplayercolor},
					sf::Color{inthighlightcolor} };
				if (ishighlighted == 1)
				{
					profilebox.highlight();
				}
				_profile_box_list.push_back(profilebox);
			}
			else if (variable_name == "Grid_On")
			{
				_showgrid = true;
			}
		}
	}
	ifs.close();
	_rightofboardwidth = _rendertexture.getSize().x - (_numcolumns * _squaresize.x + _leftofboardwidth);
	_bottomofboardheight = _rendertexture.getSize().y - (_numrows * _squaresize.y + _topofboardheight);
	return true;
}

string Board::getInitFileName(string type) const
{
	return "resources\\config_files\\" + type + ".txt";
}

string Board::getInitImageFileName(string name) const
{
	return ("resources\\setupboards\\" + name + ".png");
}

string Board::getInitSetupFileName(string name) const
{
	return ("resources\\setupboards\\setup\\" + name + ".txt");
}

string Board::getImageFileName(string name) const
{
	return ("savedboards\\" + name + ".png");
}

string Board::getSetupFileName(string name) const
{
	return ("savedboards/setup/" + name + ".txt");
}

std::string Board::getFontFileName()
{
	return "resources\\fonts\\Raleway-SemiBold.ttf";
}

sf::Vector2f Board::getPosition(BoardComponent::Coord coord)
{
	return coord.getPixelPosition(
		sf::Vector2f{ (float)_leftofboardwidth, (float)_topofboardheight },
		sf::Vector2f{ (float)_squaresize.x, (float)_squaresize.y });
}

sf::Vector2f Board::getCenterPosition(BoardComponent::Coord coord)
{
	sf::Vector2f centerpoint
	{
		(float)_squaresize.x / 2.f,
		(float)_squaresize.y / 2.f
	};
	return getPosition(coord) + centerpoint;
}

sf::Vector2f Board::getDisplayPosition(BoardComponent::Coord coord)
{
	return coord.getPixelPosition(
		getDisplaySquareOffset(),
		getDisplaySquareSize());
}

sf::Vector2f Board::getDisplayCenterPosition(BoardComponent::Coord coord)
{
	sf::Vector2f centerpoint
	{
		getDisplaySquareSize().x / 2.f,
		getDisplaySquareSize().y / 2.f
	};

	return getDisplayPosition(coord) + centerpoint;
}

bool Board::isTopLeft(BoardComponent::Coord coord) const
{
	return isValidSquare(coord) &&
		!isValidSquare(coord + BoardComponent::Coord{ sf::Vector2i{-1, 0} }) &&
		!isValidSquare(coord + BoardComponent::Coord{ sf::Vector2i{0, -1} });
}

bool Board::isBottomLeft(BoardComponent::Coord coord) const
{
	return isValidSquare(coord) &&
		!isValidSquare(coord + BoardComponent::Coord{ sf::Vector2i{-1, 0} }) &&
		!isValidSquare(coord + BoardComponent::Coord{ sf::Vector2i{0, 1} });
}

bool Board::isTopRight(BoardComponent::Coord coord) const
{
	return isValidSquare(coord) &&
		!isValidSquare(coord + BoardComponent::Coord{ sf::Vector2i{1, 0} }) &&
		!isValidSquare(coord + BoardComponent::Coord{ sf::Vector2i{0, -1} });
}

bool Board::isBottomRight(BoardComponent::Coord coord) const
{
	return isValidSquare(coord) &&
		!isValidSquare(coord + BoardComponent::Coord{ sf::Vector2i{1, 0} }) &&
		!isValidSquare(coord + BoardComponent::Coord{ sf::Vector2i{0, 1} });
}

bool Board::drawCoordinates()
{
	sf::Font font;
	if (!(font.loadFromFile(getFontFileName())))
	{
		return false;
	}
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(_notationsize);
	text.setFillColor(_textcolor);
	
	for (unsigned int x{ 0 }; x < _numcolumns; x++)
	{
		BoardComponent::Coord coord{ sf::Vector2i{(int)x, 0} };
		string filetext = coord.getNotationX();
		text.setString(filetext);
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.0f,
			0.f);

		float xpos = (float)getImageSquareOffset().x +
			(float)x*_squaresize.x +
			(float)_squaresize.x / 2.f;
		float ypos = (float)_rendertexture.getSize().y -
			float(_bottomofboardheight) +
			(float)_textbelowboardoffset;
		text.setPosition(xpos, ypos);
		_rendertexture.draw(text);
	}

	for (unsigned int y{ 0 }; y < _numrows; y++)
	{
		BoardComponent::Coord coord{ sf::Vector2i{0, (int)y} };
		string filetext = coord.getNotationY(_numrows);
		text.setString(filetext);
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(textRect.left + textRect.width / 2.f,
			textRect.top + textRect.height / 2.f);

		float xpos = (float)_leftofboardwidth / 2.f;
		float ypos = (float)_topofboardheight +
			(float)y * _squaresize.y +
			(float)_squaresize.y / 2.f;
		text.setPosition(xpos, ypos);
		_rendertexture.draw(text);
	}
	
	return true;
}