#include "instance.h"

//variable initialization, command processing and data reading method
void Instance::Init(std::vector<std::string>& argv_list_)
{
	m_input_file_name = argv_list_[1];
	m_output_file_name = argv_list_[2];

	if (argv_list_[0] == "X")
	{
		m_field.SetSide(SideCross);
		if (!m_field.LoadFromFile(m_input_file_name)) return;

		m_state = true;
		return;
	}

	if (argv_list_[0] == "0")
	{
		m_field.SetSide(SideTick);
		if (!m_field.LoadFromFile(m_input_file_name)) return;

		m_state = true;
		return;
	}

	if (argv_list_[0] == "INFO")
	{
		m_info_step = true;
		if (!WriteInfo()) return;

		m_state = true;
		return;
	}
}

//main solution-finding method
bool Instance::Run()
{
	if (m_info_step == true) return true;
	if (m_state == false) return false;

	int move_row = 0, move_col = 0;
	SolutionDepth(0, move_row, move_col);

	return WriteData(move_row, move_col);
}

void Instance::SolutionDepth(int iteration_, int& result_row_, int& result_col_)
{
	if (!m_field.SetDepth(iteration_)) return;

	Scanner scanner(&m_field);
	std::vector<Pattern> patterns;
	std::vector<Pattern> offensive_patterns;
	std::vector<Pattern> defensive_patterns;

	scanner.Scan(patterns);

#ifdef _DEBUG
	std::cout << "Iteration: " << iteration_ << "\n";
	for (auto pattern : patterns)
	{
		for (int i = 0; i < 5; i++)
		{
			if (pattern[i] == CellPlayer) std::cout << "P ";
			if (pattern[i] == CellEnemy) std::cout << "E ";
			if (pattern[i] == CellEmpty) std::cout << ". ";
			if (pattern[i] == CellOutside) std::cout << "# ";
		}
		std::cout << "\n";
	}
#endif // _DEBUG

	FindBestPatterns(offensive_patterns, defensive_patterns, patterns);
	MakeDecision(offensive_patterns, defensive_patterns, result_row_, result_col_);
}

void Instance::FindBestPatterns(std::vector<Pattern>& offensive_patterns_, std::vector<Pattern>& defensive_patterns_, std::vector<Pattern>& all_patterns_)
{
	int valuable_pattern_offensive_value = 0;
	int valuable_pattern_defensive_value = 0;

	for (auto pattern : all_patterns_) // Scanning for the best offensive and defensive patterns
	{
		int current_offensive_value = 0;
		int current_defensive_value = 0;

		if (pattern.GetValue(IMPORTANCE) > 0) 
		{
			current_offensive_value = pattern.GetValue(IMPORTANCE);
			current_defensive_value = pattern.GetValue(IMPORTANCE);
		}
		else
		{
			for (int i = 0; i < 5; i++) // Scan for offensive patterns
			{
				if (pattern.GetValue(OFFENSIVE) == -1) break;

				if (pattern[i] == CellPlayer)
				{
					current_offensive_value += 26;
					continue;
				}

				if (pattern[i] == CellEnemy || pattern[i] == CellOutside)
				{
					current_offensive_value = -1;
					break;
				}
			}

			for (int i = 0; i < 5; i++) // Scan for defensive patterns
			{
				if (pattern.GetValue(DEFENSIVE) == -1) break;

				if (pattern[i] == CellEnemy)
				{
					current_defensive_value += 26;
					continue;
				}

				if (pattern[i] == CellPlayer || pattern[i] == CellOutside)
				{
					current_defensive_value = -1;
					break;
				}
			}

			pattern.SetValue(current_offensive_value, OFFENSIVE);
			pattern.SetValue(current_defensive_value, DEFENSIVE);
		}

		if (current_offensive_value >= 104	) // On the next move there will be my win
		{
			offensive_patterns_.clear();
			offensive_patterns_.push_back(pattern);

			defensive_patterns_.clear();
			break;
		}

		if (current_offensive_value == valuable_pattern_offensive_value)
			offensive_patterns_.push_back(pattern);

		if (current_offensive_value > valuable_pattern_offensive_value)
		{
			offensive_patterns_.clear();
			offensive_patterns_.push_back(pattern);
			valuable_pattern_offensive_value = current_offensive_value;
		}

		if (current_defensive_value == valuable_pattern_defensive_value)
			defensive_patterns_.push_back(pattern);

		if (current_defensive_value > valuable_pattern_defensive_value)
		{
			defensive_patterns_.clear();
			defensive_patterns_.push_back(pattern);
			valuable_pattern_defensive_value = current_defensive_value;
		}
	}
}

void Instance::MakeDecision(std::vector<Pattern>& offensive_patterns_, std::vector<Pattern>& defensive_patterns_, int& move_row_, int& move_col_)
{
	if (offensive_patterns_.empty() && defensive_patterns_.empty())
	{
		if (m_field.IsEmpty()) // Initial move
		{
			move_row_ = 4;
			move_col_ = 4;
		}
		else
		{
			move_row_ = 0;
			move_col_ = 0;
			int pattern_quality = 0;

			int first_empty_row = -1;
			int first_empty_col = -1;

			for(int row = 0; row < 10; row++)
				for(int col = 0; col < 10; col++)
					if (m_field.GetCell(row, col) == CellEmpty)
					{
						if (first_empty_row == -1) first_empty_row = row;
						if (first_empty_col == -1) first_empty_col = col;

						std::vector<Pattern> temp_patterns;
						Scanner scanner(&m_field);

						scanner.PatternPoint(row, col, temp_patterns, true);

						if (temp_patterns.size() > pattern_quality)
						{
							pattern_quality = temp_patterns.size();
							move_row_ = row;
							move_col_ = col;
						}
					}

			if (pattern_quality == 0)
			{
				move_row_ = first_empty_row;
				move_col_ = first_empty_col;
			}
		}

		return;
	}

	int valuable_pattern_offensive_value = 0;
	int valuable_pattern_defensive_value = 0;
	if(!offensive_patterns_.empty()) valuable_pattern_offensive_value = offensive_patterns_[0].GetValue(OFFENSIVE);
	if(!defensive_patterns_.empty()) valuable_pattern_defensive_value = defensive_patterns_[0].GetValue(DEFENSIVE);

	Pattern pattern;
	if (valuable_pattern_offensive_value < valuable_pattern_defensive_value)
		pattern = defensive_patterns_[0];
	else
		pattern = offensive_patterns_[0];

	int move_value = 0;
	int good_move_index = 0;

	for (int i = 0; i < 5; i++)
	{
		if (pattern[i] == CellEmpty)
		{
			Pattern temp_pattern(pattern);
			temp_pattern[i] = CellPlayer;

			Scanner scanner(&m_field);
			scanner.ValidatePattern(temp_pattern);

			int temp_value = std::max(temp_pattern.GetValue(OFFENSIVE), 
				std::max(temp_pattern.GetValue(DEFENSIVE), temp_pattern.GetValue(IMPORTANCE)));

			if (move_value < temp_value)
			{
				move_value = temp_value;
				good_move_index = i;
			}
		}
	}

	move_row_ = pattern.m_row;
	move_col_ = pattern.m_col;

	int dir = pattern.m_dir;
	bool positive = dir > 0;
	if (dir == DIR_RIGHT || dir == DIR_LEFT)
		move_col_ = positive ? move_col_ + good_move_index : move_col_ - good_move_index;

	if (dir == DIR_TOP || dir == DIR_BOTTOM)
		move_row_ = positive ? move_row_ - good_move_index : move_row_ + good_move_index;

	if (dir == DIR_TOP_RIGHT || dir == DIR_BOTTOM_LEFT)
	{
		move_col_ = positive ? move_col_ + good_move_index : move_col_ - good_move_index;
		move_row_ = positive ? move_row_ - good_move_index : move_row_ + good_move_index;
	}

	if (dir == DIR_TOP_LEFT || dir == DIR_BOTTOM_RIGHT)
	{
		move_col_ = positive ? move_col_ - good_move_index : move_col_ + good_move_index;
		move_row_ = positive ? move_row_ - good_move_index : move_row_ + good_move_index;
	}
}

//write move information into output file
bool Instance::WriteData(int row_, int col_)
{
	std::fstream output_file(m_output_file_name, std::ios::out);
	if (!output_file.is_open()) return false;

	output_file << TranslateMove(row_, col_);

	output_file.close();
	return true;
}

//write information on INFO command
bool Instance::WriteInfo()
{
	std::fstream output_file(m_output_file_name, std::ios::out);
	if (!output_file.is_open()) return false;

	output_file << "Kirill Trofimov\n";
	output_file << "C2";

	output_file.close();
	return true;
}

//translate position into letters A-J and numbers 0-9
std::string Instance::TranslateMove(int row, int col)
{
	std::string temp_string("");
	char letter[2];
	letter[0] = 'A' + col;
	letter[1] = '\0';
	temp_string.append(letter);
	temp_string.append(std::to_string(row));

	return temp_string;
}