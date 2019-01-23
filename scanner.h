#pragma once

#include "field.h"

class Scanner
{
public:
	Scanner(Field* field_) 
	{
		m_state = true;
		if (!Init(field_)) m_state = false;
	}
	~Scanner() {}

	bool Scan();
	void Pattern_point();
	void Pattern_dir();

private:
	bool Init(Field* field_);
	bool m_state;

	Field* m_field = nullptr;
};