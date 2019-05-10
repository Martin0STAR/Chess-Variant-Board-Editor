#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include "PieceColor.h"
#include "PieceAccessory.h"
class Piece : public sf::Drawable
{
public:
	Piece();
	Piece(std::string style, std::string name, PieceColor color, std::string flags);
	Piece(const Piece &);
	~Piece();
	Piece& operator =(const Piece& rhs);
	bool operator ==(const Piece &) const;
	friend std::istream & operator >> (std::istream &is, Piece& piece);
	friend std::ostream & operator << (std::ostream &os, const Piece& piece);

	std::string getStyle() const;
	std::string getName() const;
	PieceColor getColor() const;
	std::string getFlags() const;
	Piece getPieceOnTop() const;
	std::vector<PieceAccessory> getAccessorylist() const;

	bool exists() const;
	bool isCarrier() const;
	bool isCarrying() const;
	bool isCarried() const;
	bool isMirrored() const;
	bool isUpsideDown() const;
	bool isInvertedColors() const;
	bool colorIsModified() const;
	bool hasAccessories() const;

	void setStyle(std::string style);
	void setName(std::string name);
	void setColor(PieceColor color);
	void setColorAdder(sf::Color color);
	void setColorSubtracter(sf::Color color);
	void setColorMultiplier(sf::Color color);
	void setWidth(unsigned int width);
	void setHeight(unsigned int height);
	void setSize(sf::Vector2u size);
	void setPosition(sf::Vector2f position);
	bool addPieceOnTop(Piece);
	bool addAccessory(std::string accessoryname, sf::Color color);
	bool removePieceOnTop();

	bool flipHorizontally();
	bool flipVertically();
	bool invertColors();

	bool updateImage();
private:

	std::string getFileName() const;

	void invertColor(sf::Image & image) const;
	void offsetColor(sf::Image & image) const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::string _style;
	std::string _name;
	PieceColor _color;

	sf::Vector2u _drawareasize;
	sf::Vector2f _drawareaposition;
	sf::Vector2f _inareaposition;
	
	bool _iscarried;
	bool _ismirrored;
	bool _isupsidedown;
	bool _isinvertedcolors;
	Piece* _pieceontop = nullptr;

	sf::Image _pieceimage;

	std::vector<PieceAccessory> _accessories;
};