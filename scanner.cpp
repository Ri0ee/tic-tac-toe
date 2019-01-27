#include "scanner.h"

bool Scanner::Init(Field* field_)
{
	m_field = field_;

	return true;
}

bool Scanner::Scan(std::vector<unsigned char>& bit_patterns_)
{
	if (m_state == false) return false;

	int fw, fh;
	m_field->GetSize(fw, fh);

	for (int row = 0; row < fh; row++)
	{
		for (int col = 0; col < fw; col++)
		{
			std::vector<unsigned char> temp_bit_patterns;
			PatternPoint(row, col, temp_bit_patterns);

			while (!temp_bit_patterns.empty())
			{
				bit_patterns_.push_back(*temp_bit_patterns.end());
				temp_bit_patterns.erase(temp_bit_patterns.end());
			}
		}
	}

	return true;
}

void Scanner::PatternPoint(int row_, int col_, std::vector<unsigned char> bit_patterns_)
{
	CellValue current_cell = m_field->GetCell(row_, col_);
	if (current_cell == CellEmpty) return;
	if (current_cell == CellOutside) return;
	if (current_cell == CellUnknown) return;

	for (int dir = 1; dir <= 3; dir++)
	{
		unsigned char res1 = PatternDir(row_, col_, dir);
		unsigned char res2 = PatternDir(row_, col_, -dir);

		if (res1 != 0) bit_patterns_.push_back(res1);
		if (res2 != 0) bit_patterns_.push_back(res2);
	}
}

unsigned char Scanner::PatternDir(int row_, int col_, int dir_)
{
	unsigned char temp_bit_pattern = 0;

	for (int offset = 0; offset < 5; offset++)
	{

	}

	return temp_bit_pattern;
}

bool Scanner::ValidatePattern(unsigned char& bit_pattern_)
{
	return true;
}