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
#include "Square.h"
#include "BoardCoord.h"
#include "Piece.h"
#include "PieceAccessory.h"
#include "Arrow.h"
#include "ProfileBox.h"
#include "PieceColor.h"

class Board
{
	public:
		Board();
		Board(std::string name);
		bool load(std::string);
		bool initLoad(std::string);
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
		sf::Color getSquareColor(BoardComponent::Coord) const;
		BoardComponent::Square getSquare(BoardComponent::Coord coord) const;
		Piece getPiece(BoardComponent::Coord coord) const;
		std::vector<BoardComponent::ProfileBox> getProfileBoxList() const;
		
		bool isWithinPixelBoard(sf::Vector2f pixelpos) const;
		bool isWithinBoard(sf::Vector2i) const;
		bool isWithinBoard(BoardComponent::Coord) const;
		bool isValidSquare(BoardComponent::Coord) const;
		bool isEmptySquare(BoardComponent::Coord) const;

		bool togglegrid();
		void removeDragArrow();

		bool addRowUp();
		bool addRowDown();
		bool addColumnLeft();
		bool addColumnRight();
		bool removeRowUp();
		bool removeRowDown();
		bool removeColumnLeft();
		bool removeColumnRight();

		bool setSquareColor(BoardComponent::Coord, sf::Color);
		bool removeSquare(BoardComponent::Coord);
		bool addHighlightSquare(BoardComponent::Coord, sf::Color);
		bool removeHighlightSquare(BoardComponent::Coord);

		bool addPiece(Piece &, BoardComponent::Coord);
		bool removePiece(BoardComponent::Coord);
		bool addPieceOnTop(Piece&, BoardComponent::Coord);
		bool removePieceOnTop(BoardComponent::Coord);
		bool addPieceAccessory(BoardComponent::Coord coord, std::string accessoryname, sf::Color color);

		bool addArrow(BoardComponent::Coord, BoardComponent::Coord, sf::Color);
		bool removeArrow(Arrow &);

		bool addProfileBox(BoardComponent::ProfileBox profilebox, BoardComponent::Coord coord);
		bool removeProfileBox(unsigned int playerid);
		bool highlightPlayerBox(unsigned int);
		
		bool movePiece(BoardComponent::Coord, BoardComponent::Coord);
		bool moveAll(BoardComponent::Coord);
		bool moveAllUp();
		bool moveAllDown();
		bool moveAllLeft();
		bool moveAllRight();
		
		bool generateColoredBoard(std::default_random_engine & rng);

		void updateImage();
		void updateDragArrow(BoardComponent::Coord, BoardComponent::Coord);
		bool updateProfileBox(unsigned int id, std::string name, sf::Color bordercolor);

		bool drawEmptyBoard();
		bool drawEmptySquare(BoardComponent::Coord);
		bool drawGrid();
		bool drawPiece(Piece&, BoardComponent::Coord);
		bool drawArrow(Arrow&);
		bool drawColoredSquare(sf::Color, BoardComponent::Coord);
		bool drawProfileBox(BoardComponent::ProfileBox);
		void draw(sf::RenderTarget &);
		
		void save(std::string);
		void newNameSave(std::string newname, std::string oldname);
	private:
		bool intern_load(std::string setupfilename);

		std::string getInitFileName(std::string) const;
		std::string getInitSetupFileName(std::string) const;
		std::string getImageFileName(std::string) const;
		std::string getSetupFileName(std::string) const;
		std::string getFontFileName();

		sf::Vector2f getPosition(BoardComponent::Coord);
		sf::Vector2f getCenterPosition(BoardComponent::Coord);
		sf::Vector2f getDisplayPosition(BoardComponent::Coord);
		sf::Vector2f getDisplayCenterPosition(BoardComponent::Coord);

		bool isTopLeft(BoardComponent::Coord) const;
		bool isBottomLeft(BoardComponent::Coord) const;
		bool isTopRight(BoardComponent::Coord) const;
		bool isBottomRight(BoardComponent::Coord) const;
		
		bool drawCoordinates();

		unsigned int _numrows;
		unsigned int _numcolumns;
		bool _showgrid;
		bool _showdragarrow;

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
		std::vector<Arrow> _arrow_list;
		std::vector<BoardComponent::ProfileBox> _profile_box_list;
};