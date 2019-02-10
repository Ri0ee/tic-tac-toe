#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "scanner.h"
#include "field.h"

class Instance
{
public:
	Instance(std::vector<std::string>& argv_list_) : m_state(false), m_info_step(false) { Init(argv_list_); }
	~Instance() {}

	bool Run();

private:
	void Init(std::vector<std::string>& argv_list_);
	bool m_state;

	void SolutionDepth(int iteration_, int& result_row_, int& result_col_);
	void FindBestPatterns(std::vector<Pattern>& offensive_patterns_, std::vector<Pattern>& defensive_patterns_, std::vector<Pattern>& all_patterns_);
	void MakeDecision(std::vector<Pattern>& offensive_patterns_, std::vector<Pattern>& defensive_patterns_, int& move_row_, int& move_col_);

	bool WriteData(int row_, int col_);
	bool WriteInfo();
	std::string TranslateMove(int row_, int col_);

	Field m_field;
	std::string m_input_file_name;
	std::string m_output_file_name;

	bool m_info_step;
};