#include "PieceHandler.h"

using namespace std;

PieceHandler::PieceHandler()
	:PieceHandler::PieceHandler{ "bulldog" }
{

}

PieceHandler::PieceHandler(std::string style)
	:_squaresize{sf::Vector2i{57, 57}}
{
	loadStyle(style);
}

bool PieceHandler::loadStyle(std::string style)
{
	StyleContainer container;
	ifstream ifs;
	ifs.open(getPieceListFileName(style));
	if (!ifs.is_open())
	{
		return false;
	}
	string variable_name;
	char c;
	while (ifs >> c)
	{
		if (c == '[')
		{
			ifs >> variable_name;
			if (variable_name == "Width")
			{
				ifs >> container.size.x;
			}
			else if (variable_name == "Height")
			{
				ifs >> container.size.y;
			}
			else if (variable_name == "Names")
			{
				vector<string> piecenames;
				string line;
				ifs >> ws;
				while (getline(ifs, line))
				{
					string names;
					stringstream ss{ line };
					ifs >> ws;
					while (getline(ss, names, ']'))
					{
						istringstream iss(names);
						copy(istream_iterator<string>(iss),
							istream_iterator<string>(),
							back_inserter(piecenames));
						ifs >> ws;
					}
					ifs >> ws;
				}
				for (auto name : piecenames)
				{
					PieceContainer piececontainer;
					piececontainer.name = name;
					string filename = getPieceImageFileName(style, name, "white");
					if (std::filesystem::exists(filename))
					{
						piececontainer.whiteimage.loadFromFile(filename);
					}
					filename = getPieceImageFileName(style, name, "black");
					if (std::filesystem::exists(filename))
					{
						piececontainer.blackimage.loadFromFile(filename);
					}
					container.pieces.push_back(piececontainer);
				}
			}
		}
	}
	ifs.close();

	//auto it = find(_styles.begin(), _styles.end(), style);
	//if (it != _styles.end)
	//{
	//	it->second.piecenames.clear();
	//}
	_styles.insert_or_assign(style, container);
	return true;
}

sf::Vector2f PieceHandler::getScale(std::string style)
{
	auto it = _styles.find(style);
	if (it != _styles.end())
	{
		auto piecesize = it->second.size;
		return sf::Vector2f
		{
			(float)_squaresize.x / (float)piecesize.x,
			(float)_squaresize.y / (float)piecesize.y
		};
	}
	return sf::Vector2f{ 1.f, 1.f };
}

string PieceHandler::getName(string style, unsigned int index)
{
	auto it = _styles.find(style);
	if (it != _styles.end())
	{
		return it->second.pieces.at(index).name;
	}
	else
	{
		return "";
	}
}

unsigned int PieceHandler::getNumPieces(string style)
{
	auto it = _styles.find(style);
	if (it != _styles.end())
	{
		return it->second.pieces.size();
	}
	else
	{
		return 0;
	}
}

sf::Image PieceHandler::getImage(string style, string name, string colorname)
{
	auto it = _styles.find(style);
	if (it != _styles.end())
	{
		auto it2 = find_if(it->second.pieces.begin(), it->second.pieces.end(), [name](PieceContainer piece)
		{
			return piece.name == name;
		});
		if (it2 != it->second.pieces.end())
		{
			if (colorname == "white")
			{
				return it2->whiteimage;
			}
			else
			{
				return it2->blackimage;
			}
		}
	}
	return sf::Image{};
}

bool PieceHandler::exists(string style, string name)
{
	auto it = _styles.find(style);
	if (it == _styles.end())
	{
		return false;
	}
	return find_if(it->second.pieces.begin(), it->second.pieces.end(), [name](PieceContainer piece)
	{
		return piece.name == name;
	}
	) != it->second.pieces.end();
}

string PieceHandler::getPieceListFileName(string style) const
{
	return "resources/config_files/pieces/" + style + ".txt";
}

string PieceHandler::getPieceImageFileName(string style, string name, string piececolor) const
{
	return "resources/pieces/" + style + "/" + piececolor + "/" + name + ".png";
}