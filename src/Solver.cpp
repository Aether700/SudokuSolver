#include "Solver.h"
#include <array>

#include <iostream>

class RowData;

struct SolverData
{
	Board answer;
	std::list<BoardCell*> cellsToConsider;
	std::list<BoardCell*>::iterator it;
	bool hasFitFlag = false; //keeps track if in the current iteration we managed to fill in a cell
};

static SolverData s_data;

bool IsArrValid(const BoardCell** arr, int cellVal)
{
	for (int i = 0; i < 9; i++)
	{
		if (arr[i]->GetValue() == cellVal)
		{
			return false;
		}
	}
	return true;
}

bool IsCellValid(const Board& b, int i, int j, int cellVal)
{
	const BoardCell* data[9];
	b.GetRow(j, data);
	if(!IsArrValid(data, cellVal))
	{
		return false;
	}

	b.GetCol(i, data);
	if (!IsArrValid(data, cellVal))
	{
		return false;
	}

	b.GetSubSquare(i / 3, j / 3, data);
	if (!IsArrValid(data, cellVal))
	{
		return false;
	}

	return true;
}

void SetCellOnBoard(const Vec2& pos, int cellVal)
{
	s_data.answer.SetCell(pos.x, pos.y, cellVal);
}

void InitializePossibleValues()
{
	BoardCell* cellData = s_data.answer.GetCellData();
	for(int i = 0; i < 9 * 9; i++)
	{
		BoardCell* currCell = &cellData[i];

		if (currCell->GetValue() == 0)
		{
			for (int j = 1; j <= 9; j++)
			{
				if (IsCellValid(s_data.answer, i % 9, i / 9, j))
				{
					currCell->AddToPossibleValues(j);
				}
			}
			s_data.cellsToConsider.push_front(currCell);
		}
	}
}

void AddCellToBoard(BoardCell* cell, int value);

void UpdateCells(BoardCell** cells, int newVal)
{
	for (int i = 0; i < 9; i++)
	{
		cells[i]->RemoveFromPossibleValues(newVal);
		if (cells[i]->GetNumPossibleValues() == 1)
		{
			AddCellToBoard(cells[i], cells[i]->GetPossibleValue(0));
		}
	}
}

void UpdateSubSquare(int i, int j, int value)
{
	BoardCell* subSquareCells[9];
	s_data.answer.GetSubSquare(i, j, subSquareCells);
	UpdateCells(subSquareCells, value);
}

void UpdateRow(int i, int value)
{
	BoardCell* row[9];
	s_data.answer.GetRow(i, row);
	UpdateCells(row, value);
}

void UpdateCol(int i, int value)
{
	BoardCell* col[9];
	s_data.answer.GetCol(i, col);
	UpdateCells(col, value);
}

void AddCellToBoard(BoardCell* cell, int value)
{
	s_data.hasFitFlag = true;
	cell->SetValue(value);

	for (std::list<BoardCell*>::iterator it = s_data.cellsToConsider.begin(); it != s_data.cellsToConsider.end(); it++)
	{
		if (*it == cell)
		{
			if (it == s_data.it)
			{
				s_data.it++;
			}
			s_data.cellsToConsider.erase(it);
			break;
		}
	}

	UpdateSubSquare(cell->GetPosition().x / 3, cell->GetPosition().y / 3, value);
	UpdateRow(cell->GetPosition().y, value);
	UpdateCol(cell->GetPosition().x, value);
}

void CheckPossibleValues(BoardCell** cellArr, BoardCell* currCell)
{
	bool* valuesFound = new bool[currCell->GetNumPossibleValues()];
	for (int i = 0; i < currCell->GetNumPossibleValues(); i++)
	{
		valuesFound[i] = false;
	}
	int numValuesFound = 0;
	int index = 0;

	for(int possibleVal : *currCell)
	{
		for(int i = 0; i < 9; i++)
		{
			if (cellArr[i] != currCell)
			{
				if (cellArr[i]->HasPossibleValue(possibleVal))
				{
					numValuesFound++;
					valuesFound[index] = true;
					break;
				}
			}
		}
		index++;
	}

	// current cell is the only possible cell in the provided array of cells which can contain one of the values
	if (numValuesFound == currCell->GetNumPossibleValues() - 1)
	{
		for(int i = 0; i < currCell->GetNumPossibleValues(); i++)
		{
			if (!valuesFound[i])
			{
				AddCellToBoard(currCell, currCell->GetPossibleValue(i));
				break;
			}
		}
	}

	delete[] valuesFound;
}

void RemovePossibleValue(BoardCell** cellArr, int valueToRemove)
{
	for(int i = 0; i < 9; i++)
	{
		cellArr[i]->RemoveFromPossibleValues(valueToRemove);
	}
}

void RemovePossibleValueFromRow(Board& b, int row, int valueToRemove)
{
	BoardCell* rowArr[9];
	b.GetRow(row, rowArr);
	RemovePossibleValue(rowArr, valueToRemove);
}

void RemovePossibleValueFromCol(Board& b, int col, int valueToRemove)
{
	BoardCell* colArr[9];
	b.GetCol(col, colArr);
	RemovePossibleValue(colArr, valueToRemove);
}

void CheckPossibleAxis(Board& b, BoardCell** subSquare)
{
	std::list<Vec2> possiblePositions[9]; // possiblePositions[i] -> list of positions possible for i + 1

	for (int i = 0; i < 9; i++)
	{
		for(int value : *subSquare[i])
		{
			possiblePositions[value - 1].push_back(subSquare[i]->GetPosition());
		}
	}

	for (int i = 0; i < 9; i++)
	{
		std::list<Vec2>& positions = possiblePositions[i];
		int x = -1;
		int y = -1;

		for (Vec2& pos : positions)
		{
			if (x == -1)
			{
				x = pos.x;
				y = pos.y;
				continue;
			}
			
			if (pos.x != x)
			{
				x = -2; // we have two different x positions so we cannot restrict the current number to a row
				if (y == -2)
				{
					break;
				}
			}

			if (pos.y != y)
			{
				y = -2; // we have two different y positions so we cannot restrict the current number to a column
				if (x == -2)
				{
					break;
				}
			}
		}

		if (x > 0)
		{
			RemovePossibleValueFromRow(b, x, i + 1);
			s_data.hasFitFlag = true;
		}
		else if (y > 0)
		{
			RemovePossibleValueFromCol(b, y, i + 1);
			s_data.hasFitFlag = true;
		}
	}
}

Board SudokuSolver::Solve(const Board& b)
{
	s_data.answer = b;
	InitializePossibleValues();

	while (s_data.cellsToConsider.size() != 0)
	{
		s_data.hasFitFlag = false;

		for (s_data.it = s_data.cellsToConsider.begin(); s_data.it != s_data.cellsToConsider.end(); s_data.it++)
		{
			BoardCell* currCell = *s_data.it;

			if (currCell->GetNumPossibleValues() == 1)
			{
				AddCellToBoard(currCell, currCell->GetPossibleValue(0));
			}
			else
			{
				//check row
				BoardCell* cellData[9];
				s_data.answer.GetRow(currCell->GetPosition().y, cellData);
				CheckPossibleValues(cellData, currCell);

				//check column
				s_data.answer.GetCol(currCell->GetPosition().x, cellData);
				CheckPossibleValues(cellData, currCell);


				//check sub square
				s_data.answer.GetSubSquare(currCell->GetPosition().x / 3, currCell->GetPosition().y / 3, cellData);
				CheckPossibleAxis(s_data.answer, cellData);
				CheckPossibleValues(cellData, currCell);

			}
			
			if (s_data.it == s_data.cellsToConsider.end())
			{
				break;
			}
		}


		if (!s_data.hasFitFlag)
		{
			std::cout << "Could not solve board\n\n";
			break;
		}
	}
	return s_data.answer;
}

bool SudokuSolver::IsSolutionCorrect(const Board& b)
{
	Board temp = b;
	BoardCell* cells = temp.GetCellData();
	for (int i = 0; i < 9 * 9; i++)
	{

		int value = cells[i].GetValue();
		if (value == 0)
		{
			return false;
		}

		cells[i].SetValue(0);
		if (!IsCellValid(temp, i % 9, i / 9, value))
		{
			return false;
		}
		cells[i].SetValue(value);
	}
	return true;
}