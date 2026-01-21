#pragma once
#include <cmath>

namespace CE
{
	template <typename Type>
	struct Vector2
	{
		Type m_x;	// X component of the vector
		Type m_y;	// Y component of the vector

		/*----------------------------------------------------------------
		| --- Constructor: Constructs the vector with default values --- |
		----------------------------------------------------------------*/
		Vector2()
			: m_x{ Type() }
			, m_y{ Type() }
		{}

		/*--------------------------------------------------------------------------
		| --- Constructor: Constructs the vector with specified x and y values --- |
		--------------------------------------------------------------------------*/
		Vector2(Type x, Type y)
			: m_x{ x }
			, m_y{ y }
		{}

		/*-----------------------------------------------------------------------------
		| --- Additive Operator: Adds two vectors together and returns the result --- |
		-----------------------------------------------------------------------------*/
		Vector2 operator+(const Vector2& other) const
		{
			return Vector2(m_x + other.m_x, m_y + other.m_y);
		}

		/*----------------------------------------------------------------------------------------
		| --- Subtractive Operator: Subtracts one vector from another and returns the result --- |
		----------------------------------------------------------------------------------------*/
		Vector2 operator-(const Vector2& other) const
		{
			return Vector2(m_x - other.m_x, m_y - other.m_y);
		}

		/*-------------------------------------------------------------------------------------------
		| --- Multiplicative Operator: Multiplies the vector by a scalar and returns the result --- |
		-------------------------------------------------------------------------------------------*/
		Vector2 operator*(Type scalar) const
		{
			return Vector2(m_x * scalar, m_y * scalar);
		}

		/*----------------------------------------------------------------------------------
		| --- Divisive Operator: Divides the vector by a scalar and returns the result --- |
		----------------------------------------------------------------------------------*/
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

		/*----------------------------------------------------------------------
		| --- Negation Operator: Negates the vector and returns the result --- |
		----------------------------------------------------------------------*/
		Vector2 operator-() const
		{
			return Vector2(-m_x, -m_y);
		}

		/*--------------------------------------------------------------------------
		| --- Additive Assignment Operator: Adds another vector to this vector --- |
		--------------------------------------------------------------------------*/
		Vector2 operator+=(const Vector2& other)
		{
			return Vector2(m_x += other.m_x, m_y += other.m_y);
		}

		/*------------------------------------------------------------------------------------
		| --- Subtractive Assignment Operator: Subtracts another vector from this vector --- |
		------------------------------------------------------------------------------------*/
		bool operator==(const Vector2& other) const
		{
			return (m_x == other.m_x && m_y == other.m_y);
		}

		/*------------------------------------------------------------------
		| --- Inequality Operator: Checks if two vectors are not equal --- |
		------------------------------------------------------------------*/
		bool operator!=(const Vector2& other) const
		{
			return (m_x != other.m_x && m_y != other.m_y);
		}

		/*-----------------------------------------------------------------------------
		| --- Length: Calculates and returns the length (magnitude) of the vector --- |
		-----------------------------------------------------------------------------*/
		float Length() const
		{
			return std::sqrt(m_x * m_x + m_y * m_y);
		}

		/*----------------------------------------------------------------
		| --- Normalize: Normalizes the vector to have a length of 1 --- |
		----------------------------------------------------------------*/
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

		/*------------------------------------------------------------------------------
		| --- Lerp: Linearly interpolates between two vectors based on parameter t --- |
		------------------------------------------------------------------------------*/
		static Vector2 Lerp(const Vector2& start, const Vector2& end, float t)
		{
			return start + (end - start) * t;
		}
	};

	// Type aliases for common vector types
	using VectorInt = Vector2<int>;
	using VectorFloat = Vector2<float>;
}