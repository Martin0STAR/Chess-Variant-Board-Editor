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
		BoardNameHandler(std::string);
		bool loadNames(std::string);
		std::string getName(unsigned int);
		std::string getInitBoardName(std::string);
		unsigned int getBoardNumber();
		unsigned int getNumBoards();
		bool setName(unsigned int index, std::string name);
		void save(int);
		void push_back(std::string);
		void remove(int);
	private:
		std::string getAvailableName(std::string);

		std::string _setupfilename;
		unsigned int _current_board;
		std::vector<std::string> _boardnamelist;
};