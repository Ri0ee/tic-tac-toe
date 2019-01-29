#include "scanner.h"

#define IMPORTANT_PATTERN_AMOUNT 4

CellValue important_patterns[IMPORTANT_PATTERN_AMOUNT][5] =
{
{CellEmpty, CellEnemy,	CellEnemy,	CellEnemy,	CellEmpty},
{CellEmpty, CellPlayer, CellPlayer, CellPlayer, CellEmpty},

//{CellEmpty, CellEnemy,	CellEmpty, CellEmpty, CellEmpty},
//{CellEmpty, CellPlayer, CellEmpty, CellEmpty, CellEmpty},

{CellEmpty, CellEnemy,	CellEnemy,	CellEmpty, CellEmpty},
{CellEmpty, CellPlayer, CellPlayer, CellEmpty, CellEmpty}
};

void Scanner::Scan(std::vector<Pattern>& patterns_)
{
	for (int row = 0; row < 10; row++)
	{
		for (int col = 0; col < 10; col++)
		{
			std::vector<Pattern> temp_patterns;
			PatternPoint(row, col, temp_patterns, false);

			while (!temp_patterns.empty())
			{
				patterns_.push_back(*(temp_patterns.end() - 1));
				temp_patterns.erase(temp_patterns.end() - 1);
			}
		}
	}
}

void Scanner::PatternPoint(int row_, int col_, std::vector<Pattern>& patterns_, bool potential_)
{
	CellValue current_cell = m_field->GetCell(row_, col_);
	if (current_cell == CellOutside) return;
	if (current_cell == CellUnknown) return;

	for (int dir = 1; dir <= 4; dir++)
	{
		Pattern p1 = PatternDir(row_, col_, dir);
		Pattern p2 = PatternDir(row_, col_, -dir);

		if (!potential_)
		{
			if (p1.GetValue(true) != -1)
			{
				if (p1[0] == CellEmpty)
				{
					if (ValidatePattern(p1) == true)
						patterns_.push_back(p1);
				}
				else patterns_.push_back(p1);
			}
			
			if (p2.GetValue(true) != -1)
			{
				if (p2[0] == CellEmpty)
				{
					if (ValidatePattern(p2) == true)
						patterns_.push_back(p2);
				}
				else patterns_.push_back(p2);
			}
		}
		else // Searching for empty patterns
		{
			if (p1.Empty() && p1.GetValue(true) != -1 && p1.GetValue(false) != -1) patterns_.push_back(p1);
			if (p2.Empty() && p2.GetValue(true) != -1 && p2.GetValue(false) != -1) patterns_.push_back(p2);
		}
	}
}

Pattern Scanner::PatternDir(int row_, int col_, int dir_)
{
	Pattern temp_pattern(row_, col_, dir_, nullptr);

	for (int offset = 0; offset < 5; offset++)
	{
		CellValue temp_cell = m_field->GetCell(row_, col_, dir_, offset);
		if (temp_cell == CellOutside)
		{
			temp_pattern.SetValue(-1, true);
			temp_pattern.SetValue(-1, false);
		}

		temp_pattern[offset] = temp_cell;
	}

	return temp_pattern;
}

bool Scanner::ValidatePattern(Pattern& pattern_)
{
	for (int i = 0; i < IMPORTANT_PATTERN_AMOUNT; i++)
	{
		bool valid1 = true;
		bool valid2 = true;

		for (int offset = 0; offset < 5; offset++)
		{
			if (pattern_[offset] != important_patterns[i][offset])
			{
				valid1 = false;
				break;
			}
		}

		for (int offset = 0; offset < 5; offset++)
			if (pattern_[offset] != important_patterns[i][4 - offset])
			{
				valid2 = false;
				break;
			}

		if (valid1 || valid2) return true;
	}
		
	return false;
}