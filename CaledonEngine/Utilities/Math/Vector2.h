#pragma once
#include <cmath>

namespace CE
{
	template <typename Type>
	struct Vector2
	{
		Type m_x;
		Type m_y;

		Vector2()
			: m_x{ Type() }
			, m_y{ Type() }
		{}

		Vector2(Type x, Type y)
			: m_x{ x }
			, m_y{ y }
		{}

		Vector2 operator+(const Vector2& other) const
		{
			return Vector2(m_x + other.m_x, m_y + other.m_y);
		}

		Vector2 operator-(const Vector2& other) const
		{
			return Vector2(m_x - other.m_x, m_y - other.m_y);
		}

		Vector2 operator*(Type scalar) const
		{
			return Vector2(m_x * scalar, m_y * scalar);
		}

		Vector2 operator/(Type scalar) const
		{
			if (scalar != Type())
			{
				return Vector2(m_x / scalar, m_y / scalar);
			}
			else
			{
				// Avoid division by zero
				return *this;
			}
		}

		Vector2 operator-() const
		{
			return Vector2(-m_x, -m_y);
		}

		Vector2 operator+=(const Vector2& other)
		{
			return Vector2(m_x += other.m_x, m_y += other.m_y);
		}

		bool operator==(const Vector2& other) const
		{
			return (m_x == other.m_x && m_y == other.m_y);
		}

		bool operator!=(const Vector2& other) const
		{
			return (m_x != other.m_x && m_y != other.m_y);
		}

		// Calculate the length (magnitude) of the vector
		float Length() const
		{
			return std::sqrt(m_x * m_x + m_y * m_y);
		}

		// Normalize the vector by making its length equal to 1
		Vector2& Normalize()
		{
			float length = Length();
			if (length != 0)
			{
				m_x /= length;
				m_y /= length;
			}
			return *this;
		}

		// Linear interpolation between two vectors
		static Vector2 Lerp(const Vector2& start, const Vector2& end, float t)
		{
			return start + (end - start) * t;
		}
	};

	using VectorInt = Vector2<int>;
	using VectorFloat = Vector2<float>;
}