#include "field.h"

bool Field::Init(int field_w_, int field_h_)
{
	if (field_w_ > MAX_FIELD_SIZE || field_h_ > MAX_FIELD_SIZE) return false;

	m_width = field_w_;
	m_height = field_h_;

	return true;
}

void Field::Shutdown()
{

}

void Field::Clear()
{
	for (int row = 0; row < m_height; row++)
		for (int col = 0; col < m_width; col++)
			m_field[col][row] = '.';
}