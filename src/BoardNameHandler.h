#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <iterator>
class BoardNameHandler
{
	public:
		BoardNameHandler();
		BoardNameHandler(std::string setupfilename);
		bool loadNames(std::string setupfilename);
		std::string getName(unsigned int id);
		std::string getInitBoardName(std::string type);
		unsigned int getBoardNumber();
		unsigned int getNumBoards();
		bool setName(unsigned int index, std::string name);
		void save(int current_board);
		void push_back(std::string name);
		void remove(int board_number);
	private:
		std::string getAvailableName(std::string name);

		std::string _setupfilename;
		unsigned int _current_board;
		std::vector<std::string> _boardnamelist;
};