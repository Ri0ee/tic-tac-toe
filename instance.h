#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "scanner.h"
#include "field.h"

class Instance
{
public:
	Instance(std::vector<std::string>& argv_list_) : m_state(false) { Init(argv_list_); }
	~Instance() {}

	bool Run();

private:
	void Init(std::vector<std::string>& argv_list_);
	bool m_state;

	void FindBestPatterns(std::vector<Pattern>& offensive_patterns_, std::vector<Pattern>& defensive_patterns_, std::vector<Pattern>& all_patterns_);
	void MakeDecision(std::vector<Pattern>& offensive_patterns_, std::vector<Pattern>& defensive_patterns_, int& move_row_, int& move_col_);

	bool WriteData(int row_, int col_);
	bool WriteInfo();
	std::string TranslateMove(int row_, int col_);

	Field m_field;
	std::string m_input_file_name;
	std::string m_output_file_name;
};