#include "Colortool.h"

using namespace std;

ColorTool::ColorTool()
	:ColorTool::ColorTool{ 0 }
{}

ColorTool::ColorTool(unsigned int index)
	:_index{ index }
{
	intern_load();
}

unsigned int ColorTool::getSize()
{
	return _colorlist.size();
}

unsigned int ColorTool::getIndex()
{
	return _index;
}

PieceColor ColorTool::getPieceColor() const
{
	return _colorlist.at(_index).piececolor;
}

PieceColor ColorTool::getPieceColor(unsigned int index) const
{
	return _colorlist.at(index).piececolor;
}

sf::Color ColorTool::getSquareColor() const
{
	return _colorlist.at(_index).squarecolor;
}

sf::Color ColorTool::getSquareColor(unsigned int index) const
{
	return _colorlist.at(index).squarecolor;
}

sf::Color ColorTool::getArrowColor() const
{
	return _colorlist.at(_index).arrowcolor;
}

sf::Color ColorTool::getArrowColor(unsigned int index) const
{
	return _colorlist.at(index).arrowcolor;
}

sf::Color ColorTool::getBorderColor() const
{
	return _colorlist.at(_index).bordercolor;
}

sf::Color ColorTool::getBorderColor(unsigned int index) const
{
	return _colorlist.at(index).bordercolor;
}

sf::Color ColorTool::getAccessoryColor() const
{
	return _colorlist.at(_index).accessorycolor;
}

sf::Color ColorTool::getAccessoryColor(unsigned int index) const
{
	return _colorlist.at(index).accessorycolor;
}

void ColorTool::setIndex(unsigned int index)
{
	_index = index;
}

void ColorTool::intern_load()
{
	ColorNode node0;
	ColorNode node1;
	ColorNode node2;
	ColorNode node3;
	ColorNode node4;
	ColorNode node5;
	ColorNode node6;
	ColorNode node7;
	ColorNode node8;
	ColorNode node9;
	ColorNode node10;

	node0.piececolor.name = "white";
	node1.piececolor.name = "black";
	node2.piececolor.name = "black";
	node3.piececolor.name = "white";
	node4.piececolor.name = "white";
	node5.piececolor.name = "white";
	node6.piececolor.name = "white";
	node7.piececolor.name = "white";
	node8.piececolor.name = "white";
	node9.piececolor.name = "white";
	node10.piececolor.name = "white";

	node0.piececolor.adder = sf::Color{ 0, 0, 0, 0 };
	node1.piececolor.adder = sf::Color{ 0, 0, 0, 0 };
	node2.piececolor.adder = sf::Color{ 100,0,0,0 };
	node3.piececolor.adder = sf::Color{ 0,0,100,0 };
	node4.piececolor.adder = sf::Color{ 50,50,0,0 };
	node5.piececolor.adder = sf::Color{ 0,100,0,0 };
	node6.piececolor.adder = sf::Color{ 100,0,0,0 };
	node7.piececolor.adder = sf::Color{ 0,100,100,0 };
	node8.piececolor.adder = sf::Color{ 0,0,0,0 };
	node9.piececolor.adder = sf::Color{ 0,0,100,0 };
	node10.piececolor.adder = sf::Color{ 100,0,0,0 };

	node0.piececolor.subtracter = sf::Color{ 0, 0, 0, 0 };
	node1.piececolor.subtracter = sf::Color{ 0, 0, 0, 0 };
	node2.piececolor.subtracter = sf::Color{ 0,100,100,0 };
	node3.piececolor.subtracter = sf::Color{ 100,100,0,0 };
	node4.piececolor.subtracter = sf::Color{ 0,0,255,0 };
	node5.piececolor.subtracter = sf::Color{ 100,0,100,0 };
	node6.piececolor.subtracter = sf::Color{ 0,25,255,0 };
	node7.piececolor.subtracter = sf::Color{ 100,0,0,0 };
	node8.piececolor.subtracter = sf::Color{ 100,100,100,0 };
	node9.piececolor.subtracter = sf::Color{ 0,100,0,0 };
	node10.piececolor.subtracter = sf::Color{ 0,0,0,0 };

	node0.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };
	node1.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };
	node2.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };
	node3.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };
	node4.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };
	node5.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };
	node6.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };
	node7.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };
	node8.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };
	node9.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };
	node10.piececolor.multiplier = sf::Color{ 255, 255, 255, 255 };

	node0.squarecolor = sf::Color{ 255, 255, 255, 255 };
	node1.squarecolor = sf::Color{ 121,94,50,255 };
	node2.squarecolor = sf::Color{ 164,63,51,255 };
	node3.squarecolor = sf::Color{ 65,64,142,255 };
	node4.squarecolor = sf::Color{ 216,214,33,255 };
	node5.squarecolor = sf::Color{ 91,145,61,255 };
	node6.squarecolor = sf::Color{ 224,144,45,255 };
	node7.squarecolor = sf::Color{ 83,147,191,255 };
	node8.squarecolor = sf::Color{ 0,0,0,255 };
	node9.squarecolor = sf::Color{ 123,0,255,255 };
	node10.squarecolor = sf::Color{ 255,192,203,255 };

	node0.arrowcolor = sf::Color{ 255,255,255,200 };
	node1.arrowcolor = sf::Color{ 121, 94, 50, 200 };
	node2.arrowcolor = sf::Color{ 255,0,0,200 };
	node3.arrowcolor = sf::Color{ 0,0,255,200 };
	node4.arrowcolor = sf::Color{ 255,223,0,200 };
	node5.arrowcolor = sf::Color{ 0,255,0,200 };
	node6.arrowcolor = sf::Color{ 255,165,0,200 };
	node7.arrowcolor = sf::Color{ 0,255,255,200 };
	node8.arrowcolor = sf::Color{ 0,0,0,200 };
	node9.arrowcolor = sf::Color{ 160,32,240,128 };
	node10.arrowcolor = sf::Color{ 255,192,203,128 };

	node0.bordercolor = sf::Color{ 190, 190, 190, 255 };
	node1.bordercolor = sf::Color{ 121,94,50, 255 };
	node2.bordercolor = sf::Color{ 255, 0, 0, 255 };
	node3.bordercolor = sf::Color{ 0 , 0, 255, 255 };
	node4.bordercolor = sf::Color{ 255, 223, 0, 255 };
	node5.bordercolor = sf::Color{ 0,255,0,255 };
	node6.bordercolor = sf::Color{ 255, 165, 0, 255 };
	node7.bordercolor = sf::Color{ 0, 255, 255, 255 };
	node8.bordercolor = sf::Color{ 100,100,100,255 };
	node9.bordercolor = sf::Color{ 160, 32, 240, 255 };
	node10.bordercolor = sf::Color{ 255, 192, 203, 255 };

	node0.accessorycolor = sf::Color{ 255, 255, 255, 255 };
	node1.accessorycolor = sf::Color{ 121,94,50, 255 };
	node2.accessorycolor = sf::Color{ 164,63,51,255 };
	node3.accessorycolor = sf::Color{ 65,64,142,255 };
	node4.accessorycolor = sf::Color{ 216,214,33,255 };
	node5.accessorycolor = sf::Color{ 91,145,61,255 };
	node6.accessorycolor = sf::Color{ 224,144,45,255 };
	node7.accessorycolor = sf::Color{ 83,147,191,255 };
	node8.accessorycolor = sf::Color{ 0,0,0,255 };
	node9.accessorycolor = sf::Color{ 160,32,240,255 };
	node10.accessorycolor = sf::Color{ 255,192,203,255 };

	_colorlist.push_back(node0);
	_colorlist.push_back(node1);
	_colorlist.push_back(node2);
	_colorlist.push_back(node3);
	_colorlist.push_back(node4);
	_colorlist.push_back(node5);
	_colorlist.push_back(node6);
	_colorlist.push_back(node7);
	_colorlist.push_back(node8);
	_colorlist.push_back(node9);
	_colorlist.push_back(node10);
}