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

	for (auto pattern : patterns)
	{
		int current_offensive_value = 0;
		for (int i = 0; i < 5; i++) // Scan for offensive patterns
		{
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

	for (auto pattern : patterns)
	{
		for (int i = 0; i < 5; i++)
			std::cout << pattern[i] << " ";
		std::cout << "\n";
	}

	std::cout << "\n";

	for (auto pattern : valuable_offensive_patterns)
	{
		for (int i = 0; i < 5; i++)
			std::cout << pattern[i] << " ";
		std::cout << "\n";
	}

	return true;
}

//write move information into output file
bool Instance::WriteData()
{
	std::fstream output_file(m_output_file_name, std::ios::out);
	if (!output_file.is_open()) return false;

	//output_file << TranslateMove(m_final_move.row, m_final_move.col) << "\n";

	output_file.close();
	return false;
}

//write information on INFO command
bool Instance::WriteInfo()
{
	std::fstream output_file(m_output_file_name, std::ios::out);
	if (!output_file.is_open()) return false;

	output_file << "Kirill Trofimov\n";
	output_file << "C2";

	output_file.close();
	return false;
}

//translate position into letters A-J and numbers 0-9
std::string Instance::TranslateMove(int row, int col)
{
	std::string temp_string("");
	temp_string.append((char*)('A' + col));
	temp_string.append(std::to_string(row));

	return temp_string;
}