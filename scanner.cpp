#include "scanner.h"

#define IMPORTANT_PATTERN_AMOUNT 14

std::vector<Pattern> important_patterns;
CellValue static_patterns[IMPORTANT_PATTERN_AMOUNT][5] =
{
{CellPlayer, CellPlayer, CellPlayer, CellPlayer, CellEmpty},	// 4
{CellEnemy, CellEnemy,	CellEnemy,	CellEnemy,	CellEmpty},		// 4
{CellPlayer, CellEmpty, CellPlayer, CellPlayer, CellPlayer},	// 4
{CellEnemy, CellEmpty, CellEnemy, CellEnemy, CellEnemy},		// 4
{CellPlayer, CellPlayer, CellEmpty, CellPlayer, CellPlayer},	// 4
{CellEnemy, CellEnemy, CellEmpty, CellEnemy, CellEnemy},		// 4

{CellEmpty,	CellEnemy,	CellEnemy,	CellEnemy,	CellEmpty},		// Open 3
{CellEmpty,	CellPlayer,	CellPlayer, CellPlayer, CellEmpty},		// Open 3

{CellEmpty,	CellEnemy,	CellEnemy,	CellEmpty,	CellEnemy},		// Open 3 Broken
{CellEnemy,	CellEnemy,	CellEmpty,	CellEnemy,	CellEmpty},		// Open 3 
{CellPlayer,CellPlayer,	CellEmpty,	CellPlayer,	CellEmpty},		// Open 3
{CellEmpty,	CellPlayer,	CellPlayer,	CellEmpty,	CellPlayer},	// Open 3 Broken

{CellEnemy,	CellPlayer,	CellPlayer, CellPlayer, CellEmpty},		// Closed 3
{CellPlayer,CellEnemy,	CellEnemy,	CellEnemy,	CellEmpty}		// Closed 3
};

void Scanner::Scan(std::vector<Pattern>& patterns_)
{
	important_patterns.push_back(Pattern(static_patterns[0], 1000));
	important_patterns.push_back(Pattern(static_patterns[1], 1000));
	important_patterns.push_back(Pattern(static_patterns[2], 1000));
	important_patterns.push_back(Pattern(static_patterns[3], 1000));
	important_patterns.push_back(Pattern(static_patterns[4], 5000));
	important_patterns.push_back(Pattern(static_patterns[5], 5000));

	important_patterns.push_back(Pattern(static_patterns[6], 400));
	important_patterns.push_back(Pattern(static_patterns[7], 400));

	//important_patterns.push_back(Pattern(static_patterns[8], 200));
	important_patterns.push_back(Pattern(static_patterns[9], 300));
	important_patterns.push_back(Pattern(static_patterns[10], 300));
	//important_patterns.push_back(Pattern(static_patterns[11], 300));

	important_patterns.push_back(Pattern(static_patterns[12], 60));
	important_patterns.push_back(Pattern(static_patterns[13], 50));

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
			if (p1.GetValue(OFFENSIVE) != -1)
			{
				if (p1[0] == CellEmpty)
				{
					if (ValidatePattern(p1) == true)
						patterns_.push_back(p1);
				}
				else patterns_.push_back(p1);
			}
			
			if (p2.GetValue(OFFENSIVE) != -1)
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
			if (p1.Empty() && p1.GetValue(OFFENSIVE) != -1 && p1.GetValue(DEFENSIVE) != -1) patterns_.push_back(p1);
			if (p2.Empty() && p2.GetValue(OFFENSIVE) != -1 && p2.GetValue(DEFENSIVE) != -1) patterns_.push_back(p2);
		}
	}
}

Pattern Scanner::PatternDir(int row_, int col_, int dir_)
{
	Pattern temp_pattern(row_, col_, dir_, nullptr);
	temp_pattern.SetValue(0, OFFENSIVE);
	temp_pattern.SetValue(0, DEFENSIVE);
	temp_pattern.SetValue(0, IMPORTANCE);

	for (int offset = 0; offset < 5; offset++)
	{
		CellValue temp_cell = m_field->GetCell(row_, col_, dir_, offset);
		if (temp_cell == CellOutside)
		{
			temp_pattern.SetValue(-1, OFFENSIVE);
			temp_pattern.SetValue(-1, DEFENSIVE);
			temp_pattern.SetValue(-1, IMPORTANCE);
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

		if (valid1 || valid2)
		{
			pattern_.SetValue(important_patterns[i].GetValue(IMPORTANCE), IMPORTANCE);
			return true;
		}
	}
		
	return false;
}