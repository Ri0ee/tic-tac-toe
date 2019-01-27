#pragma once

#include <vector>

#include "field.h"

class Scanner
{
public:
	Scanner(Field* field_) 
	{
		m_state = true;
		if (!Init(field_)) m_state = false;
	}

	~Scanner() {}

	bool Scan(std::vector<unsigned char>& bit_patterns_);
	void PatternPoint(int row_, int col_, std::vector<unsigned char> bit_patterns_);
	unsigned char PatternDir(int row_, int col_, int dir_);
	bool ValidatePattern(unsigned char& bit_pattern_);

private:
	bool Init(Field* field_);
	bool m_state;

	Field* m_field = nullptr;
};