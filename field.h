#pragma once

#define MAX_FIELD_SIZE 20

class Field
{
public:
	Field() {}
	~Field() {}

	bool Init(int field_w_, int field_h_);
	void Shutdown();
	void Clear();

	void GetSize(int& field_w_, int& field_h_) {
		field_w_ = m_width;
		field_h_ = m_height;
	}

	void SetSize(int field_w_, int field_h_) {
		if (field_w_ > 0 && field_w_ <= MAX_FIELD_SIZE) m_width = field_w_;
		if (field_h_ > 0 && field_h_ <= MAX_FIELD_SIZE) m_height = field_h_;
	}

	char GetCell(int row_, int col_)
	{
		if (col_ >= 0 && col_ <= m_width && row_ >= 0 && row_ <= m_height)
			return m_field[col_][row_];
		else
			return -1;
	}

	void SetCell(int row_, int col_, char value_)
	{
		if (col_ >= 0 && col_ <= m_width && row_ >= 0 && row_ <= m_height)
			m_field[col_][row_] = value_;
	}

private:
	int m_width, m_height;
	char m_field[MAX_FIELD_SIZE][MAX_FIELD_SIZE];
};