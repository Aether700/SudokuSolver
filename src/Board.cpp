#include "Board.h"

// Vec2 ///////////////////////////////////////////////
Vec2::Vec2() : x(-1), y(-1) { }
Vec2::Vec2(int _x, int _y) : x(_x), y(_y) { }

// BoardCell //////////////////////////////////////////

BoardCell::BoardCell() : m_cellValue(-1) { }
BoardCell::BoardCell(const Vec2& pos, int value) : m_pos(pos), m_cellValue(value) { }

int BoardCell::GetValue() const { return m_cellValue; }

void BoardCell::SetValue(int value) { m_cellValue = value; }

const Vec2& BoardCell::GetPosition() const { return m_pos; }

size_t BoardCell::GetNumPossibleValues() const
{
	return m_possibleValues.size();
}

int BoardCell::GetPossibleValue(int index) const
{
	std::list<int>::const_iterator it = m_possibleValues.begin();
	for(int i = 0; i < index; i++)
	{
		it++;
	}
	return *it;
}

bool BoardCell::HasPossibleValue(int value) const
{
	for (std::list<int>::const_iterator it = m_possibleValues.begin(); it != m_possibleValues.end(); it++)
	{
		if (*it == value)
		{
			return true;
		}
		else if (*it > value)
		{
			return false;
		}
	}
	return false;
}

std::list<int>::const_iterator BoardCell::begin() const { return m_possibleValues.begin(); }
std::list<int>::const_iterator BoardCell::end() const { return m_possibleValues.end(); }

void BoardCell::AddToPossibleValues(int value)
{
	std::list<int>::iterator it = m_possibleValues.begin();
	for (; it != m_possibleValues.end(); it++)
	{
		if (*it == value) 
		{
			//do not add to the list if the value is already contained
			return;
		}
		else if (*it > value)
		{
			m_possibleValues.insert(it++, value);
			return;
		}
	}
	m_possibleValues.insert(it, value);
}

void BoardCell::RemoveFromPossibleValues(int value)
{
	if (m_possibleValues.size() == 0)
	{
		return;
	}

	for (std::list<int>::iterator it = m_possibleValues.begin(); it != m_possibleValues.end(); it++) 
	{
		if (*it == value)
		{
			m_possibleValues.erase(it);
			return;
		}
		else if (*it > value)
		{
			//value is not in the list so just return and do nothing
			return;
		}
	}
}


// Board //////////////////////////////////////////////////////

Board::Board() { }

Board::Board(const std::string& filepath)
{
	std::ifstream file = std::ifstream(filepath);
	assert(file);

	for (int i = 0; i < 9 * 9; i++)
	{
		char c;
		file.read(&c, sizeof(char));
		m_boardCells[i] = BoardCell(Vec2(i % 9, i / 9), atoi(&c));
	}
}

BoardCell* Board::GetCellData() { return m_boardCells; }

BoardCell* Board::GetCell(int i, int j)
{
	return &(m_boardCells[i + 9 * j]);
}

const BoardCell* Board::GetCell(int i, int j) const
{
	return &(m_boardCells[i + 9 * j]);
}

void Board::SetCell(int i, int j, int val)
{
	m_boardCells[i + 9 * j].SetValue(val);
}

void Board::GetRow(int row, BoardCell** dest)
{
	for (int i = 0; i < 9; i++)
	{
		dest[i] = &m_boardCells[i + 9 * row];
	}
}

void Board::GetRow(int row, const BoardCell** dest) const
{
	for (int i = 0; i < 9; i++)
	{
		dest[i] = &m_boardCells[i + 9 * row];
	}
}

void Board::GetCol(int col, BoardCell** dest)
{
	for (int i = 0; i < 9; i++)
	{
		dest[i] = &m_boardCells[col + 9 * i];
	}
}

void Board::GetCol(int col, const BoardCell** dest) const
{
	for (int i = 0; i < 9; i++)
	{
		dest[i] = &m_boardCells[col + 9 * i];
	}
}

void Board::GetSubSquare(int i, int j, BoardCell** dest)
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			dest[y + 3 * x] = &m_boardCells[(x + i * 3) + ((y + j * 3) * 9)];
		}
	}
}

void Board::GetSubSquare(int i, int j, const BoardCell** dest) const
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			dest[y + 3 * x] = &m_boardCells[(x + i * 3) + ((y + j * 3) * 9)];
		}
	}
}

std::string Board::ToString() const
{
	std::stringstream ss;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			int cell = (m_boardCells[j + 9 * i]).GetValue();
			if (cell == 0)
			{
				ss << "_ ";
			}
			else
			{
				ss << cell << " ";
			}
		}
		ss << "\n";
	}
	return ss.str();
}