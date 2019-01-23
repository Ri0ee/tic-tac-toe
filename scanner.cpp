#include "scanner.h"

bool Scanner::Init(Field* field_)
{
	m_field = field_;

	return true;
}

bool Scanner::Scan()
{
	if (m_state == false) return false;

	int fw, fh;
	m_field->GetSize(fw, fh);

	for (int row = 0; row < fh; row++)
	{
		for (int col = 0; col < fw; col++)
		{
			Pattern_point();
		}
	}

	return true;
}