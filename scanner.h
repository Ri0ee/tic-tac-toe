#pragma once

#include <vector>

#include "field.h"

struct Pattern
{
	Pattern() : m_offensive_value(0), m_defensive_value(0), m_dir(0), m_col(0), m_row(0)
	{
		for (int i = 0; i < 5; i++) m_pattern[i] = CellUnknown;
	}

	Pattern(int row_, int col_, int dir_, CellValue pattern_[5]) : m_offensive_value(0), m_defensive_value(0), m_row(row_), m_col(col_), m_dir(dir_)
	{
		if (pattern_ != nullptr)
			for (int i = 0; i < 5; i++) m_pattern[i] = pattern_[i];
		else 
			for (int i = 0; i < 5; i++) m_pattern[i] = CellUnknown;
	}

	bool operator==(Pattern& pattern_)
	{
		for(int i = 0; i < 5; i++)
			if (pattern_.m_pattern[i] != m_pattern[i]) return false;

		return true;
	}

	bool operator!=(Pattern& pattern_)
	{
		for (int i = 0; i < 5; i++)
			if (pattern_.m_pattern[i] != m_pattern[i]) return true;

		return false;
	}

	CellValue& operator[](int index_)
	{
		return m_pattern[index_];
	}

	bool Exists()
	{
		for (int i = 0; i < 5; i++)
			if (m_pattern[i] != CellUnknown) return false;

		return true;
	}

	bool Empty() 
	{
		for (int i = 0; i < 5; i++)
			if (m_pattern[i] != CellEmpty) return false;

		return true;
	}

	int GetValue(bool offensive_)
	{
		if (offensive_) return m_offensive_value;
		return m_defensive_value;
	}

	void SetValue(int value_, bool offensive_)
	{
		if (offensive_)
			m_offensive_value = value_;
		else
			m_defensive_value = value_;
	}

	int m_row;
	int m_col;
	int m_dir;

private:
	int m_offensive_value;
	int m_defensive_value;
	CellValue m_pattern[5];
};

class Scanner
{
public:
	Scanner(Field* field_) : m_field(field_) {}
	~Scanner() {}

	void Scan(std::vector<Pattern>& patterns_);
	void PatternPoint(int row_, int col_, std::vector<Pattern>& patterns_, bool potential_);
	Pattern PatternDir(int row_, int col_, int dir_);
	bool ValidatePattern(Pattern& pattern_);

private:
	Field* m_field = nullptr;
};