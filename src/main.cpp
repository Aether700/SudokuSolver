#include <iostream>
#include "Board.h"
#include "Solver.h"

int main()
{
	Board b = Board("res/test_board2.sdm");
	Board answer = SudokuSolver::Solve(b);

	std::cout << "Initial board:\n";
	std::cout << b.ToString() << "\n\n";
	std::cout << "Solution board:\n";
	std::cout << answer.ToString() << "\n";
	std::cout << "Solution is correct: " << SudokuSolver::IsSolutionCorrect(answer) << "\n";
}