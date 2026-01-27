#pragma once

namespace CE
{
	struct Color
	{
		unsigned char m_r;
		unsigned char m_g;
		unsigned char m_b;
		unsigned char m_a;

		constexpr Color(unsigned char red = 255, unsigned char green = 255,
			unsigned char blue = 255, unsigned char alpha = 255)
			: m_r{ red }
			, m_g{ green }
			, m_b{ blue }
			, m_a{ alpha }
		{}

		// Predefined colors
		static constexpr Color Red()		{ return Color{ 255, 0, 0, 255 }; }
		static constexpr Color Green()		{ return Color{ 0, 255, 0, 255 }; }
		static constexpr Color Blue()		{ return Color{ 0, 0, 255, 255 }; }
		static constexpr Color White()		{ return Color{ 255, 255, 255, 255 }; }
		static constexpr Color Black()		{ return Color{ 0, 0, 0, 255 }; }
		static constexpr Color Yellow()		{ return Color{ 255, 255, 0, 255 }; }
		static constexpr Color Magenta()	{ return Color{ 255, 0, 255, 255 }; }
		static constexpr Color Cyan()		{ return Color{ 0, 255, 255, 255 }; }

		// Comparison operators
		bool operator==(const Color& other) const
		{
			return m_r == other.m_r && m_g == other.m_g && m_b == other.m_b && m_a == other.m_a;
		}

		bool operator!=(const Color& other) const
		{
			return !(*this == other);
		}
	};
}