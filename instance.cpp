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
		if (!WriteInfo()) return;

		m_state = true;
		return;
	}
}

//main solution-finding method
bool Instance::Run()
{
	if (m_state == false) return false;

	std::vector<Pattern> valuable_offensive_patterns;
	std::vector<Pattern> valuable_defensive_patterns;
	int valuable_pattern_offensive_value = 0;
	int valuable_pattern_defensive_value = 0;

	Scanner scanner(&m_field);
	std::vector<Pattern> patterns;
	scanner.Scan(patterns);

#ifdef _DEBUG
	for (auto pattern : patterns)
	{
		for (int i = 0; i < 5; i++)
		{
			if (pattern[i] == CellPlayer) std::cout << "P ";
			if (pattern[i] == CellEnemy) std::cout << "N ";
			if (pattern[i] == CellEmpty) std::cout << ". ";
			if (pattern[i] == CellOutside) std::cout << "# ";
		}
		std::cout << "\n";
	}
#endif // _DEBUG

	for (auto pattern : patterns) // Scanning for the best offensive and defensive patterns
	{
		int current_offensive_value = 0;
		for (int i = 0; i < 5; i++) // Scan for offensive patterns
		{
			if (pattern.GetValue() == -1) break;

			if (pattern[i] == CellPlayer)
			{
				current_offensive_value++;
				continue;
			}

			if (pattern[i] == CellEnemy || pattern[i] == CellOutside)
			{
				current_offensive_value = -1;
				break;
			}
		}

		if (current_offensive_value == valuable_pattern_offensive_value)
			valuable_offensive_patterns.push_back(pattern);

		if (current_offensive_value > valuable_pattern_offensive_value)
		{
			valuable_offensive_patterns.clear();
			valuable_offensive_patterns.push_back(pattern);
			valuable_pattern_offensive_value = current_offensive_value;
		}

		int current_defensive_value = 0;
		for (int i = 0; i < 5; i++) // Scan for defensive patterns
		{
			if (pattern.GetValue() == -1) break;

			if (pattern[i] == CellEnemy)
			{
				current_defensive_value++;
				continue;
			}

			if (pattern[i] == CellPlayer || pattern[i] == CellOutside)
			{
				current_defensive_value = -1;
				break;
			}
		}

		if (current_defensive_value == valuable_pattern_defensive_value)
			valuable_defensive_patterns.push_back(pattern);

		if (current_defensive_value > valuable_pattern_defensive_value)
		{
			valuable_defensive_patterns.clear();
			valuable_defensive_patterns.push_back(pattern);
			valuable_pattern_defensive_value = current_defensive_value;
		}
	}

	int move_row, move_col;
	for (int i = 4; i > 0; i--)
	{
		Pattern temp_pattern;
		if (valuable_pattern_defensive_value < valuable_pattern_offensive_value)
			temp_pattern = valuable_offensive_patterns[0];
		else
			temp_pattern = valuable_defensive_patterns[0];

		if (temp_pattern[i] == CellEmpty)
		{
			move_row = temp_pattern.m_row;
			move_col = temp_pattern.m_col;

			int dir = temp_pattern.m_dir;
			bool positive = dir > 0;
			if (dir == DIR_RIGHT || dir == DIR_LEFT)
				move_col = positive ? move_col + i : move_col - i;

			if (dir == DIR_TOP || dir == DIR_BOTTOM)
				move_row = positive ? move_row - i : move_row + i;

			if (dir == DIR_TOP_RIGHT || dir == DIR_BOTTOM_LEFT)
			{
				move_col = positive ? move_col + i : move_col - i;
				move_row = positive ? move_row - i : move_row + i;
			}

			if (dir == DIR_TOP_LEFT || dir == DIR_BOTTOM_RIGHT)
			{
				move_col = positive ? move_col - i : move_col + i;
				move_row = positive ? move_row - i : move_row + i;
			}

			break;
		}
	}

	return WriteData(move_row, move_col);;
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