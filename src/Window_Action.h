#pragma once

#include <string>
#include "Piece.h"
#include "BoardCoord.h"
#include "Tool.h"

namespace Window_Action_State {
	enum Window_Action_State
	{
		NOTHING, CLOSE_WINDOW, CLOSE_PIECE_WINDOW,
		SAVE_BOARD, LOAD_BOARD,
		LOAD_BOARD_LEFT, LOAD_BOARD_RIGHT,
		ADD_BOARD, DELETE_BOARD,
		ADD_ROW_UP, ADD_ROW_DOWN,
		ADD_COLUMN_LEFT, ADD_COLUMN_RIGHT,
		REMOVE_ROW_UP, REMOVE_ROW_DOWN,
		REMOVE_COLUMN_LEFT, REMOVE_COLUMN_RIGHT,
		FLIP_BOARD_HORIZONTALLY,
		FLIP_BOARD_VERTICALLY,
		ADD_PROFILE_BOX, REMOVE_PROFILE_BOX,
		SET_PROFILE_BOX,
		SET_TOOL_STATE,
		SET_TOOL_PIECE_STYLE,
		SET_TOOL_PIECE_TYPE, SET_TOOL_COLOR,
		SET_TOOL_PIECE,
		SET_TOOL_ADD_PIECE_ACCESSORY,
		SWAP_TOOLS,
		ADD_PIECE, REMOVE_PIECE,
		MOVE_PIECE,
		FLIP_PIECE_HORIZONTAL, FLIP_PIECE_VERTICAL,
		INVERT_PIECE_COLOR,
		SET_SQUARE_COLOR,
		SET_PIECE_ACCESSORY,
		SET_PIECE_CHAR_ACCESSORY,
		REMOVE_SQUARE,
		ADD_LINE,
		ADD_ARROW,
		ADD_PIECE_ACCESSORY,
		ADD_PIECE_CHAR_ACCESSORY,
		HIGHLIGHT_SQUARE, HIGHLIGHT_PLAYERBOX,
		UPDATE_DRAG_ARROW,
		REMOVE_DRAG_ARROW,
		PICK_PIECE,
		TOGGLE_GRID,
		TOGGLE_GRID_SELECT,
		GENERATE_COLORED_BOARD,
		ENTER_BOARD_NAME,
		DISPLAY_ENTERED_BOARD_NAME,
		SET_BOARD_NAME,
		ENTER_PROFILE_NAME,
		DISPLAY_ENTERED_PROFILE_NAME,
		SET_PROFILE_IMAGE,
		ENTER_SAVE_AS_BOARDNAME,
		DISPLAY_SAVE_AS_BOARDNAME,
		SAVE_BOARD_AS
	};
}


struct Window_Action
{
	Window_Action_State::Window_Action_State state{Window_Action_State::NOTHING};
	unsigned int board_number;
	unsigned int player_id;
	bool isLeftTool;
	BoardComponent::Coord fromsquarecoord;
	BoardComponent::Coord tosquarecoord;
	Piece piece;
	sf::Color squarecolor;
	sf::Color arrowcolor;
	sf::Color bordercolor;
	unsigned int colorindex;
	std::string name;
	Tool_State::Tool_State toolstate;
};