#include "instance.h"

//variable initialization, command processing and data reading method
bool Instance::Init(std::vector<std::string>& argv_list_)
{
	m_input_file_name = argv_list_[1];
	m_output_file_name = argv_list_[2];

	if (argv_list_[0] == "X")
	{
		m_field.SetSide(SideCross);
		if (!m_field.LoadFromFile(m_input_file_name)) return false;
		return true;
	}

	if (argv_list_[0] == "0")
	{
		m_field.SetSide(SideTick);
		if (!m_field.LoadFromFile(m_input_file_name)) return false;
		return true;
	}

	if (argv_list_[0] == "INFO")
	{
		if (!WriteInfo()) return false;
		return true;
	}

	return false; //command unknown
}

//main solution-finding method
bool Instance::Run()
{
	if (m_state == false) return false;

	Scanner scanner(&m_field);
	std::vector<unsigned char> bit_patterns;

	scanner.Scan(bit_patterns);

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