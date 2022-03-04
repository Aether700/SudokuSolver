#pragma once
#include "Board.h"

class SudokuSolver
{
public:
	static Board Solve(const Board& b);
	static bool IsSolutionCorrect(const Board& b);
};