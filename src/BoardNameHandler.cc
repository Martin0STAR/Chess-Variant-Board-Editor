#include "BoardNameHandler.h"

using namespace std;

BoardNameHandler::BoardNameHandler()
{
	_current_board = -1; //undefined.
	//Need to run LoadNames before usable
}

BoardNameHandler::BoardNameHandler(string setupfilename)
{
	loadNames(setupfilename);
}

bool BoardNameHandler::loadNames(string setupfilename)
{
	_setupfilename = setupfilename;
	ifstream ifs;
	ifs.open(setupfilename);
	string variable_name;
	char c;
	while (ifs >> c)
	{
		if (c == '[')
		{
			ifs >> variable_name;
			if (variable_name == "CurrentBoard")
			{
				ifs >> _current_board;
			}
			else if (variable_name == "BoardNames")
			{
				string line;
				ifs >> ws;
				while (getline(ifs, line))
				{
					string boardnames;
					stringstream ss{ line };
					ifs >> ws;
					while (getline(ss, boardnames,']'))
					{
						istringstream iss(boardnames);
						copy(istream_iterator<string>(iss),
							istream_iterator<string>(),
							back_inserter(_boardnamelist));
						ifs >> ws;
					}
					ifs >> ws;
				}
			}
		}
	}
	ifs.close();
	return _boardnamelist.size() > 0;
}

string BoardNameHandler::getName(unsigned int id)
{
	if(getNumBoards() == 0 || getNumBoards() > _boardnamelist.size())
	{
		return "undefined";
	}
	return _boardnamelist.at(id - 1);
}

std::string BoardNameHandler::getInitBoardName(string type)
{
	return type;
}

unsigned int BoardNameHandler::getBoardNumber()
{
	return _current_board;
}

unsigned int BoardNameHandler::getNumBoards()
{
	return _boardnamelist.size();
}

bool BoardNameHandler::setName(unsigned int index, string name)
{
	if (name == "" || name == getName(index))
	{
		return false;
	}
	_boardnamelist.at(index-1) = getAvailableName(name);
	return true;
}

void BoardNameHandler::save(int current_board)
{
	ofstream ofs;
	ofs.open(_setupfilename);
	ofs << "[CurrentBoard " << current_board << "]\n";
	ofs << "[BoardNames ";
	for (auto it = _boardnamelist.begin(); it != _boardnamelist.end(); it++)
	{
		ofs << *it;
		if (it + 1 != _boardnamelist.end())
		{
			ofs << " ";
		}
	}
	ofs << "]";
}

void BoardNameHandler::push_back(string name)
{
	_boardnamelist.push_back(getAvailableName(name));
}

void BoardNameHandler::remove(int board_number)
{
	string name = _boardnamelist.at(board_number-1);
	std::remove(("savedboards\\" + name + ".png").c_str());
	std::remove(("savedboards/setup/" + name + ".txt").c_str());
	_boardnamelist.erase(_boardnamelist.begin() + board_number - 1);
}

string BoardNameHandler::getAvailableName(string name)
{
	ifstream testname("savedboards\\" + name + ".png");
	if (!(testname.good()))
	{
		return name;
	}
	for (int i{ 2 }; i < 100; i++)
	{
		ifstream testname2("savedboards\\" + name +
			to_string(i) + ".png");
		if (!(testname2.good()))
		{
			return name + to_string(i);
		}
	}
	return "";
}