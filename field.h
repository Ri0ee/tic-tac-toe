#pragma once

#include <string>
#include <fstream>
#include <iostream>

#define MAX_FIELD_SIZE 20
#define MAX_FIELD_DEPTH 5

#define DIR_RIGHT			1
#define DIR_LEFT			-1
#define DIR_TOP				2
#define DIR_BOTTOM			-2
#define DIR_TOP_RIGHT		3
#define DIR_BOTTOM_LEFT		-3
#define DIR_TOP_LEFT		4
#define DIR_BOTTOM_RIGHT	-4

enum CellValue {
	CellPlayer,
	CellEnemy,
	CellEmpty,
	CellOutside,
	CellUnknown
};

enum Side {
	SideCross,
	SideTick,
	SideUnknown
};

class Field
{
public:
	Field(Side side_) : m_side(side_), m_depth(0) {}
	Field() : m_side(SideUnknown), m_depth(0) {}
	~Field() {}

	bool LoadFromFile(std::string& file_name_);
	void Clear(int depth_); // depth_ = -1 clears all levels of field array

	void SetSide(Side side_) 
	{
		m_side = side_;
	}

	CellValue GetCell(int row_, int col_)
	{
		if (col_ >= 0 && col_ < 10 && row_ >= 0 && row_ < 10)
			return m_field[col_][row_][m_depth];
		else return CellOutside;
	}

	CellValue GetCell(int row_, int col_, int dir_, int offset)
	{
		bool positive = dir_ > 0;
		if (dir_ == DIR_RIGHT || dir_ == DIR_LEFT)
			return positive ? GetCell(row_, col_ + offset) : GetCell(row_, col_ - offset);

		if (dir_ == DIR_TOP || dir_ == DIR_BOTTOM)
			return positive ? GetCell(row_ - offset, col_) : GetCell(row_ + offset, col_);

		if (dir_ == DIR_TOP_RIGHT || dir_ == DIR_BOTTOM_LEFT)
			return positive ? GetCell(row_ - offset, col_ + offset) : GetCell(row_ + offset, col_ - offset);

		if (dir_ == DIR_TOP_LEFT || dir_ == DIR_BOTTOM_RIGHT)
			return positive ? GetCell(row_ - offset, col_ - offset) : GetCell(row_ + offset, col_ + offset);

		return CellUnknown;
	}

	void SetCell(int row_, int col_, CellValue value_)
	{
		if (col_ >= 0 && col_ <= 10 && row_ >= 0 && row_ <= 10)
			m_field[col_][row_][m_depth] = value_;
	}

	bool IsEmpty()
	{
		for (int row = 0; row < 10; row++)
			for (int col = 0; col < 10; col++)
				if (GetCell(row, col) != CellEmpty) return false;
		return true;
	}

	bool SetDepth(int depth_)
	{
		if (depth_ < MAX_FIELD_DEPTH)
		{
			if (m_depth_initialization_status[depth_] == false)
			{
				CopyLayer(depth_ - 1, depth_);
				m_depth_initialization_status[depth_] = true;
			}
			m_depth = depth_;
		}
		else
			return false;

		return true;
	}

	int GetDepth()
	{
		return m_depth;
	}

	void CopyLayer(int src_depth_, int dest_depth_)
	{
		for (int row = 0; row < 10; row++)
			for (int col = 0; col < 10; col++)
				m_field[row][col][dest_depth_] = m_field[row][col][src_depth_];
	}

#ifdef _DEBUG
	void DebugOutput();
#endif // _DEBUG

private:
	Side m_side;
	CellValue m_field[MAX_FIELD_SIZE][MAX_FIELD_SIZE][MAX_FIELD_DEPTH];
	int m_depth;
	bool m_depth_initialization_status[MAX_FIELD_DEPTH];
};