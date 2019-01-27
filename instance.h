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

	bool WriteData();
	bool WriteInfo();
	std::string TranslateMove(int row_, int col_);

	Field m_field;
	std::string m_input_file_name;
	std::string m_output_file_name;
};