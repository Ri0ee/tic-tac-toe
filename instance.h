#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "scanner.h"
#include "field.h"

#define PLAYER_SIDE_UNKNOWN 0
#define PLAYER_SIDE_CROSS 1
#define PLAYER_SIDE_TICK 2

class Instance
{
public:
	Instance(std::vector<std::string>& argv_list_) 
	{
		m_state = true;
		if (!Init(argv_list_)) m_state = false;
	}
	~Instance() {}

	bool Run();

private:
	bool Init(std::vector<std::string>& argv_list_);
	bool m_state;

	bool ReadData();
	bool WriteData();
	bool WriteInfo();
	std::string TranslateMove(int row_, int col_);

	int m_player_side;
	Field m_field;
	std::string m_input_file_name;
	std::string m_output_file_name;
};