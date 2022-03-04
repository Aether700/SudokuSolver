#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <list>

struct Vec2
{
	int x;
	int y;

	Vec2();
	Vec2(int _x, int _y);
};

class BoardCell
{
public:
	BoardCell();
	BoardCell(const Vec2& pos, int value);

	int GetValue() const;
	void SetValue(int value);

	const Vec2& GetPosition() const;

	size_t GetNumPossibleValues() const;
	int GetPossibleValue(int index) const;

	bool HasPossibleValue(int value) const;

	std::list<int>::const_iterator begin() const;
	std::list<int>::const_iterator end() const;

	void AddToPossibleValues(int value);
	void RemoveFromPossibleValues(int value);

private:
	int m_cellValue;
	std::list<int> m_possibleValues; // sorted list of the possible values for the given cell
	Vec2 m_pos;
};

class Board
{
public:
	Board();
	Board(const std::string& filepath);
	BoardCell* GetCellData();
	BoardCell* GetCell(int i, int j);
	const BoardCell* GetCell(int i, int j) const;
	void SetCell(int i, int j, int val);
	void GetCol(int col, BoardCell** dest);
	void GetCol(int col, const BoardCell** dest) const;
	void GetRow(int row, BoardCell** dest);
	void GetRow(int row, const BoardCell** dest) const;
	void GetSubSquare(int i, int j, BoardCell** dest);
	void GetSubSquare(int i, int j, const BoardCell** dest) const;
	std::string ToString() const;

private:
	BoardCell m_boardCells[9 * 9];
};