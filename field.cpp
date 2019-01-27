#include "field.h"

#ifdef _DEBUG
void Field::DebugOutput()
{
	for(int row = 0; row < m_height; row++)
	{
		for (int col = 0; col < m_width; col++)
		{
			CellValue temp_cell = GetCell(row, col);
			
			if (temp_cell == CellEmpty)
			{
				std::cout << ".";
				continue;
			}

			if (temp_cell == CellEnemy)
			{
				std::cout << "E";
				continue;
			}

			if (temp_cell == CellPlayer)
			{
				std::cout << "P";
				continue;
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n";

	std::cout << "GetCell: " << GetCell(5, 4, 2, 1) << "\n";
}
#endif // _DEBUG

bool Field::Init(int field_w_, int field_h_, Side side_)
{
	if (field_w_ > MAX_FIELD_SIZE || field_h_ > MAX_FIELD_SIZE) return false;

	m_width = field_w_;
	m_height = field_h_;
	m_side = side_;

	return true;
}

void Field::Clear()
{
	for (int row = 0; row < m_height; row++)
		for (int col = 0; col < m_width; col++)
			m_field[col][row] = CellEmpty;
}

bool Field::LoadFromFile(std::string& file_name_)
{
	if (m_side == SideUnknown) return false;

	std::fstream input_file(file_name_, std::ios::in);
	if (!input_file.is_open()) return false;

	for (int row = 0; row < 10; row++)
	{
		std::string line;
		std::getline(input_file, line);
		for (int col = 0; col < 10; col++)
		{
			if (line[col] == '.')
			{
				SetCell(row, col, CellEmpty);
				continue;
			}

			if ((m_side == SideCross && line[col] == 'X') || (m_side == SideTick && line[col] == '0'))
			{
				SetCell(row, col, CellPlayer);
				continue;
			}
			else
			{
				SetCell(row, col, CellEnemy);
				continue;
			}
		}
	}

#ifdef _DEBUG
	DebugOutput();
#endif // _DEBUG

	input_file.close();
	return true;
}