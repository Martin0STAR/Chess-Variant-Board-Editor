#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Board.h"
#include <array>
#include "PieceHandler.h"
#include "Piece.h"
#include "Tool.h"

namespace ToolWindowComponent
{
	enum Icon
	{
		LEFTTOOL,
		RIGHTTOOL,
		ARROWLEFT,
		ARROWRIGHT,
		SAVE,
		ADDBOARD,
		REMOVEBOARD
	};
}

class ToolWindow : public sf::Drawable
{
public:
	ToolWindow();
	ToolWindow(Board&);
	void load(Board&);
	Tool& getTool(bool);
	sf::Vector2u getSize() const;

	bool isIconArea(const ToolWindowComponent::Icon & icon, sf::Vector2f pixelcoord) const;
	bool isSaved();
	void setPieceHandler(PieceHandler* piecehandler);
	void setTool(bool islefttool, Tool_State::Tool_State state);
	void setSaveIconSaved();
	void setSaveIconNotSaved();
	void swapTools();
	bool updateToolImage(bool islefttool);
	
private:
	
	std::string getSaveIconFileNameSaved() const;
	std::string getSaveIconFileNameNotSaved() const;
	std::string getArrowRightFileName() const;
	std::string getAddIconFileName() const;
	std::string getDeleteIconFileName() const;
	
	sf::Image getSavedImage() const;
	sf::Image getNotSavedImage() const;
	sf::Image getArrowRightImage() const;
	sf::Image getArrowLeftImage() const;
	sf::Image getAddImage() const;
	sf::Image getDeleteImage() const;
	sf::Vector2f getToolPosition(const unsigned int & index) const;
	bool setPosition(sf::Sprite & sprite, const unsigned int & index);
	bool updateArrowLeftImage();
	bool updateArrowRightImage();
	bool updateAddImage();
	bool updateDeleteImage();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	unsigned int _boardheight;
	sf::Vector2u _squaresize;
	unsigned int _numicons;
	float _yoffset;
	float _xoffset;
	Tool _lefttool;
	Tool _righttool;
	sf::Color _backgroundcolor;
	sf::RenderTexture _rendertexture;
	bool _modified;
};