/*------------------------------
| File: Rect.h
| Author: Chandler Mays
------------------------------*/
#pragma once

namespace CE
{
	struct Rect
	{
		int m_x;
		int m_y;
		int m_width;
		int m_height;

		Rect(int xPos = 0, int yPos = 0, int w = 0, int h = 0)
			: m_x{ xPos }
			, m_y{ yPos }
			, m_width{ w }
			, m_height{ h }
		{ }

		int Left() const		{ return m_x; }
		int Right() const		{ return m_x + m_width; }
		int Top() const			{ return m_y; }
		int Bottom() const		{ return m_y + m_height; }

		int CenterX() const		{ return m_x + m_width / 2; }
		int CenterY() const		{ return m_y + m_height / 2; }

		bool Contains(int pointX, int pointY) const
		{
			return pointX >= m_x && pointX < m_x + m_width &&
				pointY >= m_y && pointY < m_y + m_height;
		}

		bool Intersects(const Rect& other) const
		{
			return !(Right() <= other.Left() || Left() >= other.Right() ||
				Bottom() <= other.Top() || Top() >= other.Bottom());
		}

		void Scale(float factor)
		{
			m_width = static_cast<int>(m_width * factor);
			m_height = static_cast<int>(m_height * factor);
		}

		bool operator==(const Rect& other) const
		{
			return m_x == other.m_x && m_y == other.m_y &&
				m_width == other.m_width && m_height == other.m_height;
		}

		bool operator!=(const Rect& other) const
		{
			return !(*this == other);
		}
	};
}