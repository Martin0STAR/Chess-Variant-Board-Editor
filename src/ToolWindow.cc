#include "ToolWindow.h"

using namespace std;

ToolWindow::ToolWindow()
{
	_boardheight = 0;
	_squaresize = sf::Vector2u{ 0,0 };
	//must run load(board) to work
}

ToolWindow::ToolWindow(Board& board)
{
	load(board);
}

void ToolWindow::load(Board& board)
{
	_boardheight = board.getDisplaySize().x;
	_squaresize = board.getImageSquareSize();
	_yoffset = 5.f;

	_rendertexture.create(
		board.getDisplaySize().y,
		_squaresize.y + (unsigned int)_yoffset * 2
	);
	_backgroundcolor = sf::Color{ 180, 180, 180, 255 };
	_rendertexture.clear(_backgroundcolor);

	_numicons = 7;
	_xoffset = (float)(_rendertexture.getSize().x - _numicons * _squaresize.x) / (float)_numicons;
	

	_lefttool.load(_squaresize, Tool_State::SELECT,
		board.getSquareColor(0), board.getBoardColor());
	_righttool.load(_squaresize, Tool_State::ADD_ARROW,
		board.getSquareColor(1), board.getBoardColor());
	_modified = true;

	updateArrowLeftImage();
	updateArrowRightImage();
	setSaveIconSaved();
	updateAddImage();
	updateDeleteImage();

	_lefttool.setState(Tool_State::SELECT);
	_lefttool.setColorIndex(0);
	_lefttool.updateImage();
	_lefttool.setPosition(getToolPosition(ToolWindowComponent::LEFTTOOL));
	_lefttool.draw(_rendertexture);
	_righttool.setState(Tool_State::ADD_ARROW);
	_righttool.setColorIndex(2);
	_righttool.updateImage();
	_righttool.setPosition(getToolPosition(ToolWindowComponent::RIGHTTOOL));
	_righttool.draw(_rendertexture);
}

Tool& ToolWindow::getTool(bool isLeftTool)
{
	if (isLeftTool)
	{
		return _lefttool;
	}
	else
	{
		return _righttool;
	}
}

sf::Vector2u ToolWindow::getSize() const
{
	return _rendertexture.getSize();
}

bool ToolWindow::isIconArea(const ToolWindowComponent::Icon & icon, sf::Vector2f pixelcoord) const
{
	auto position = getToolPosition(icon);
	auto leftx = position.x;
	auto rightx = leftx + _squaresize.x;
	auto topy = position.y;
	auto bottomy = topy + _squaresize.y;
	return pixelcoord.x >= leftx &&
		pixelcoord.x < rightx &&
		pixelcoord.y >= topy &&
		pixelcoord.y < bottomy;
}

bool ToolWindow::isSaved()
{
	return !_modified;
}

void ToolWindow::setTool(bool islefttool, Tool_State::Tool_State state)
{
	if (islefttool)
	{
		_lefttool.setState(state);
	}
	else
	{
		_righttool.setState(state);
	}
}

void ToolWindow::setSaveIconSaved()
{
	if(_modified)
	{
		_modified = false;
		sf::Image image = getSavedImage();
		sf::Texture texture;
		texture.loadFromImage(image);
		sf::Sprite sprite{ texture };
		setPosition(sprite, ToolWindowComponent::SAVE);
		_rendertexture.draw(sprite);
	}
}

void ToolWindow::swapTools()
{
	std::swap(_lefttool, _righttool);
	sf::Color temp{ _righttool.getBackgroundColor() };
	_righttool.setSquareColor(_lefttool.getBackgroundColor());
	_lefttool.setSquareColor(temp);
}

void ToolWindow::setSaveIconNotSaved()
{
	if(!(_modified))
	{
		_modified = true;
		sf::Image image = getNotSavedImage();
		sf::Texture texture;
		texture.loadFromImage(image);
		sf::Sprite sprite{ texture };
		setPosition(sprite, ToolWindowComponent::SAVE);
		_rendertexture.draw(sprite);
	}
}

bool ToolWindow::updateToolImage(bool islefttool)
{
	if (islefttool)
	{
		_lefttool.updateImage();
		_lefttool.setPosition(getToolPosition(ToolWindowComponent::LEFTTOOL));
		_lefttool.draw(_rendertexture);
		return true;
	}
	else
	{
		_righttool.updateImage();
		_righttool.setPosition(getToolPosition(ToolWindowComponent::RIGHTTOOL));
		_righttool.draw(_rendertexture);
		return true;
	}
}

void ToolWindow::draw(sf::RenderTarget & target)
{
	_rendertexture.display();
	sf::Sprite sprite{ _rendertexture.getTexture() };
	sprite.setPosition(0.f, static_cast<float>(_boardheight));
	target.draw(sprite);
}

string ToolWindow::getSaveIconFileNameSaved() const
{
	return "resources/icons/graysave.png";
}

string ToolWindow::getSaveIconFileNameNotSaved() const
{
	return "resources/icons/save.png";
}

string ToolWindow::getArrowRightFileName() const
{
	return "resources/icons/arrow_right.png";
}

string ToolWindow::getAddIconFileName() const
{
	return "resources/icons/add_icon.png";
}

string ToolWindow::getDeleteIconFileName() const
{
	return "resources/icons/delete_icon.png";
}

sf::Image ToolWindow::getSavedImage() const
{
	sf::Image image;
	image.loadFromFile(getSaveIconFileNameSaved());
	return image;
}


sf::Image ToolWindow::getNotSavedImage() const
{
	sf::Image image;
	image.loadFromFile(getSaveIconFileNameNotSaved());
	return image;
}

sf::Image ToolWindow::getArrowRightImage() const
{
	sf::Image image;
	image.loadFromFile(getArrowRightFileName());
	return image;
}

sf::Image ToolWindow::getArrowLeftImage() const
{
	sf::Image image = getArrowRightImage();
	image.flipHorizontally();
	return image;
}

sf::Image ToolWindow::getAddImage() const
{
	sf::Image image;
	image.loadFromFile(getAddIconFileName());
	return image;
}

sf::Image ToolWindow::getDeleteImage() const
{
	sf::Image image;
	image.loadFromFile(getDeleteIconFileName());
	return image;
}

sf::Vector2f ToolWindow::getToolPosition(const unsigned int & index) const
{
	if (index == 0)
	{
		return sf::Vector2f{ _xoffset, _yoffset };
	}
	return sf::Vector2f{
		(float)(index * _xoffset + index * _squaresize.x),
		(float)_yoffset };
}

bool ToolWindow::setPosition(sf::Sprite & sprite, const unsigned int & index)
{
	sprite.setPosition(getToolPosition(index));
	return true;
}

bool ToolWindow::updateArrowLeftImage()
{
	sf::Image leftarrow = getArrowLeftImage();
	sf::Texture texture;
	if (!(texture.loadFromImage(leftarrow)))
	{
		return false;
	}
	sf::Sprite sprite{ texture };
	setPosition(sprite, ToolWindowComponent::ARROWLEFT);
	_rendertexture.draw(sprite);
	return true;
}

bool ToolWindow::updateArrowRightImage()
{
	sf::Image rightarrow = getArrowRightImage();
	sf::Texture texture;
	if (!(texture.loadFromImage(rightarrow)))
	{
		return false;
	}
	sf::Sprite sprite{ texture };
	setPosition(sprite, ToolWindowComponent::ARROWRIGHT);
	_rendertexture.draw(sprite);
	return true;
}

bool ToolWindow::updateAddImage()
{
	sf::Image add_icon = getAddImage();
	sf::Texture texture;
	if (!(texture.loadFromImage(add_icon)))
	{
		return false;
	}
	sf::Sprite sprite{ texture };
	setPosition(sprite, ToolWindowComponent::ADDBOARD);
	_rendertexture.draw(sprite);
	return true;
}

bool ToolWindow::updateDeleteImage()
{
	sf::Image deleteicon = getDeleteImage();
	sf::Texture texture;
	if (!(texture.loadFromImage(deleteicon)))
	{
		return false;
	}
	sf::Sprite sprite{ texture };
	setPosition(sprite, ToolWindowComponent::REMOVEBOARD);
	_rendertexture.draw(sprite);
	return true;
}