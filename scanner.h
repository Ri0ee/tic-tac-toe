#pragma once

#include <vector>

#include "field.h"

#define OFFENSIVE 1
#define DEFENSIVE 2
#define IMPORTANCE 3

struct Pattern
{
	Pattern() : m_offensive_value(0), m_defensive_value(0), m_dir(0), m_col(0), m_row(0)
	{
		for (int i = 0; i < 5; i++) m_pattern[i] = CellUnknown;
	}

	Pattern(int row_, int col_, int dir_, CellValue pattern_[5]) : m_offensive_value(0), m_defensive_value(0), m_importance_value(0), m_row(row_), m_col(col_), m_dir(dir_)
	{
		if (pattern_ != nullptr)
			for (int i = 0; i < 5; i++) m_pattern[i] = pattern_[i];
		else 
			for (int i = 0; i < 5; i++) m_pattern[i] = CellUnknown;
	}

	Pattern(const CellValue pattern_[5], int value_) : m_offensive_value(0), m_defensive_value(0), m_importance_value(value_), m_row(0), m_col(0), m_dir(0)
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

	int GetValue(int type_)
	{
		if (type_ == OFFENSIVE) return m_offensive_value;
		if (type_ == DEFENSIVE) return m_defensive_value;
		if (type_ == IMPORTANCE) return m_importance_value;
		return -1;
	}

	void SetValue(int value_, int type_)
	{
		if (type_ == OFFENSIVE) m_offensive_value = value_;
		else if (type_ == DEFENSIVE) m_defensive_value = value_;
		else if (type_ == IMPORTANCE) m_importance_value = value_;
	}

	int m_row;
	int m_col;
	int m_dir;

private:
	int m_offensive_value;
	int m_defensive_value;
	int m_importance_value;
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