#include "scanner.h"

void Scanner::Scan(std::vector<Pattern>& patterns_)
{
	for (int row = 0; row < 10; row++)
	{
		for (int col = 0; col < 10; col++)
		{
			std::vector<Pattern> temp_patterns;
			PatternPoint(row, col, temp_patterns);

			while (!temp_patterns.empty())
			{
				patterns_.push_back(*(temp_patterns.end() - 1));
				temp_patterns.erase(temp_patterns.end() - 1);
			}
		}
	}
}

void Scanner::PatternPoint(int row_, int col_, std::vector<Pattern>& patterns_)
{
	CellValue current_cell = m_field->GetCell(row_, col_);
	if (current_cell == CellEmpty) return;
	if (current_cell == CellOutside) return;
	if (current_cell == CellUnknown) return;

	for (int dir = 1; dir <= 4; dir++)
	{
		Pattern p1 = PatternDir(row_, col_, dir);
		Pattern p2 = PatternDir(row_, col_, -dir);

		if (!p1.IsEmpty() && p1.GetValue() != -1) patterns_.push_back(p1);
		if (!p2.IsEmpty() && p2.GetValue() != -1) patterns_.push_back(p2);
	}
}

Pattern Scanner::PatternDir(int row_, int col_, int dir_)
{
	Pattern temp_pattern(row_, col_, dir_, nullptr);

	bool failed_pattern = false;
	for (int offset = 0; offset < 5; offset++)
	{
		CellValue temp_cell = m_field->GetCell(row_, col_, dir_, offset);
		if (temp_cell == CellOutside)
			temp_pattern.SetValue(-1);

		temp_pattern[offset] = temp_cell;
	}

	return temp_pattern;
}

bool Scanner::ValidatePattern(Pattern& pattern_)
{
	return true;
}