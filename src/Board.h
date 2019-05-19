#pragma once
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <filesystem>
#include <utility>
#include <algorithm>
#include <random>
#include <array>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "PieceHandler.h"
#include "Square.h"
#include "BoardCoord.h"
#include "Piece.h"
#include "PieceAccessory.h"
#include "Line.h"
#include "Arrow.h"
#include "ProfileBox.h"
#include "PieceColor.h"

class Board : public sf::Drawable
{
	public:
		Board();
		Board(PieceHandler* piecehandler);
		Board(PieceHandler* piecehandler, std::string name);
		bool load(std::string name);
		bool initLoad(std::string name);
		~Board();

		sf::Vector2u getImageSize() const;
		sf::Vector2u getDisplaySize() const;
		sf::Vector2u getImageSquareSize() const;
		sf::Vector2f getDisplaySquareSize() const;
		sf::Vector2u getImageSquareOffset() const;
		sf::Vector2f getDisplaySquareOffset() const;

		sf::Vector2i getImageCoordFromDisplayCoord(sf::Vector2f displaypos) const;

		unsigned int getNumRows() const;
		unsigned int getNumColumns() const;
		sf::Color getBoardColor() const;
		sf::Color getSquareColor(unsigned int index) const;
		sf::Color getSquareColor(BoardComponent::Coord coord) const;
		BoardComponent::Square getSquare(BoardComponent::Coord coord) const;
		Piece getPiece(BoardComponent::Coord coord) const;
		std::vector<BoardComponent::ProfileBox> getProfileBoxList() const;
		
		bool isWithinPixelBoard(sf::Vector2f pixelpos) const;
		bool isWithinBoard(sf::Vector2i coord) const;
		bool isWithinBoard(BoardComponent::Coord coord) const;
		bool isValidSquare(BoardComponent::Coord coord) const;
		bool isEmptySquare(BoardComponent::Coord coord) const;

		bool flipHorizontally();
		bool flipVertically();
		bool togglegrid();
		void removeDragArrow();

		void setPieceHandler(PieceHandler* piecehandler);

		bool addRowUp();
		bool addRowDown();
		bool addColumnLeft();
		bool addColumnRight();
		bool removeRowUp();
		bool removeRowDown();
		bool removeColumnLeft();
		bool removeColumnRight();

		bool setSquareColor(BoardComponent::Coord coord, sf::Color color);
		bool removeSquare(BoardComponent::Coord coord);
		bool addHighlightSquare(BoardComponent::Coord coord, sf::Color color);
		bool removeHighlightSquare(BoardComponent::Coord coord);

		bool addPiece(Piece &, BoardComponent::Coord coord);
		bool removePiece(BoardComponent::Coord coord);
		bool addPieceOnTop(Piece&, BoardComponent::Coord coord);
		bool removePieceOnTop(BoardComponent::Coord coord);
		bool addPieceAccessory(BoardComponent::Coord coord, std::string accessoryname, sf::Color color);

		bool addLine(BoardComponent::Coord fromcoord, BoardComponent::Coord tocoord, sf::Color color);
		bool removeLine(Line& line);

		bool addArrow(BoardComponent::Coord fromcoord, BoardComponent::Coord tocoord, sf::Color color);
		bool removeArrow(Arrow & arrow);

		bool addProfileBox(BoardComponent::ProfileBox profilebox, BoardComponent::Coord coord);
		bool removeProfileBox(unsigned int playerid);
		bool highlightPlayerBox(unsigned int playerid);
		
		bool movePiece(BoardComponent::Coord fromcoord, BoardComponent::Coord tocoord);
		bool moveAll(BoardComponent::Coord movecoord);
		bool moveAllUp();
		bool moveAllDown();
		bool moveAllLeft();
		bool moveAllRight();
		
		bool generateColoredBoard(std::default_random_engine & rng);

		void updateImage();
		void updateDragArrow(BoardComponent::Coord fromcoord, BoardComponent::Coord tocoord);
		bool updateProfileBox(unsigned int id, std::string name, sf::Color bordercolor);

		bool drawEmptyBoard();
		bool drawEmptySquare(BoardComponent::Coord coord);
		bool drawGrid();
		bool drawPiece(Piece& piece, BoardComponent::Coord coord);
		bool drawLine(Line& line);
		bool drawArrow(Arrow& arrow);
		bool drawColoredSquare(sf::Color color, BoardComponent::Coord coord);
		bool drawProfileBox(BoardComponent::ProfileBox profilebox);
		
		void save(std::string name);
		void newNameSave(std::string newname, std::string oldname);
	private:
		bool intern_load(std::string setupfilename);

		std::string getInitFileName(std::string type) const;
		std::string getInitSetupFileName(std::string name) const;
		std::string getImageFileName(std::string name) const;
		std::string getSetupFileName(std::string name) const;
		std::string getFontFileName();

		sf::Vector2f getPosition(BoardComponent::Coord);
		sf::Vector2f getCenterPosition(BoardComponent::Coord);
		sf::Vector2f getDisplayPosition(BoardComponent::Coord);
		sf::Vector2f getDisplayCenterPosition(BoardComponent::Coord);

		bool isTopLeft(BoardComponent::Coord coord) const;
		bool isBottomLeft(BoardComponent::Coord coord) const;
		bool isTopRight(BoardComponent::Coord coord) const;
		bool isBottomRight(BoardComponent::Coord coord) const;
		
		bool drawCoordinates();

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		PieceHandler* _piecehandler;

		unsigned int _numrows;
		unsigned int _numcolumns;
		bool _showgrid;
		bool _showdragarrow;
		bool _ishorizontalflipped;
		bool _isverticalflipped;

		sf::RenderTexture _rendertexture;
		Arrow _dragarrow;
		
		sf::Color _boardcolor;

		sf::Vector2u _displaysize;
		unsigned int _topofboardheight;
		unsigned int _bottomofboardheight;
		unsigned int _leftofboardwidth;
		unsigned int _rightofboardwidth;
		unsigned int _boardcornersize;
		
		sf::Vector2u _squaresize;
		unsigned int _squarecornersize;

		unsigned int _notationsize;
		sf::Color _notationcolor;

		std::vector<sf::Color> _squarecolors;
		std::vector<BoardComponent::Coord> _removed_square_list;
		std::map<BoardComponent::Coord, sf::Color> _colored_square_list;
		std::map<BoardComponent::Coord, sf::Color> _highlighted_square_list;
		std::map<BoardComponent::Coord, Piece> _piece_list;
		std::vector<Line> _line_list;
		std::vector<Arrow> _arrow_list;
		std::vector<BoardComponent::ProfileBox> _profile_box_list;
};