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
							back_inserter(container.piecenames));
						ifs >> ws;
					}
					ifs >> ws;
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

vector<string> PieceHandler::getNameList(string style)
{
	auto it = _styles.find(style);
	if (it != _styles.end())
	{
		return it->second.piecenames;
	}
	return vector<string>{};
}

string PieceHandler::getPieceListFileName(string style)
{
	return "resources/config_files/pieces/" + style + ".txt";
}