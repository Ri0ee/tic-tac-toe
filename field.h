#pragma once

#include <string>
#include <fstream>
#include <iostream>

#define MAX_FIELD_SIZE 20

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
	Field(int field_w_, int field_h_, Side side_) : m_width(field_w_), m_height(field_h_), m_side(side_) {}
	Field() : m_width(10), m_height(10), m_side(SideUnknown) {}
	~Field() {}

	bool LoadFromFile(std::string& file_name_);
	void Clear();

	void SetSide(Side side_) {
		m_side = side_;
	}

	void GetSize(int& field_w_, int& field_h_) {
		field_w_ = m_width;
		field_h_ = m_height;
	}

	void SetSize(int field_w_, int field_h_) {
		if (field_w_ > 0 && field_w_ <= MAX_FIELD_SIZE) m_width = field_w_;
		if (field_h_ > 0 && field_h_ <= MAX_FIELD_SIZE) m_height = field_h_;
	}

	CellValue GetCell(int row_, int col_)
	{
		if (col_ >= 0 && col_ < m_width && row_ >= 0 && row_ < m_height)
			return m_field[col_][row_];
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
		if (col_ >= 0 && col_ <= m_width && row_ >= 0 && row_ <= m_height)
			m_field[col_][row_] = value_;
	}

#ifdef _DEBUG
	void DebugOutput();
#endif // _DEBUG

private:
	Side m_side;
	int m_width, m_height;
	CellValue m_field[MAX_FIELD_SIZE][MAX_FIELD_SIZE];
};