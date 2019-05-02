#include "PieceColor.h"

bool operator==(const PieceColor& lhs, const PieceColor& rhs)
{
	return lhs.name == rhs.name &&
		lhs.adder == rhs.adder &&
		lhs.subtracter == rhs.subtracter &&
		lhs.multiplier == rhs.multiplier;
}