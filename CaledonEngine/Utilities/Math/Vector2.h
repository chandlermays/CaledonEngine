#pragma once
#include <cmath>
#include <algorithm>
#include <cassert>
#include <string>

namespace CE
{
	template <typename T>
	class Vector2
	{
	public:
		static constexpr size_t N = 2;																							// Number of components in the vector

		T x;																													// The x component of the vector
		T y;																													// The y component of the vector

		inline Vector2()																										// Default constructor initializes the vector to (0, 0)
			: x{ 0 }
			, y{ 0 }
		{ }

		constexpr inline Vector2(const Vector2& other) = default;																// Copy constructor

		constexpr inline Vector2(const T x, const T y)																			// Constructor that initializes the vector with specified x and y values
			: x{ x }
			, y{ y }
		{ }

		constexpr inline explicit Vector2(const T uniformValue)																	// Constructor that initializes both components of the vector to the same value
			: x{ uniformValue }
			, y{ uniformValue }
		{ }

		constexpr inline Vector2& operator=(const Vector2& other) = default;													// Copy assignment operator

		inline bool operator==(const Vector2 other) const				{ return (x == other.x && y == other.y); }				// Compare two vectors for equality
		inline bool operator!=(const Vector2 other) const				{ return !(*this == other); }							// Compare two vectors for inequality
		inline bool operator<(const Vector2 other) const				{ return (x < other.x && y < other.y); }				// Compare if this vector is less than another vector (component-wise)
		inline bool operator>(const Vector2 other) const				{ return (x > other.x && y > other.y); }				// Compare if this vector is greater than another vector (component-wise)
		inline bool operator<=(const Vector2 other) const				{ return (x <= other.x && y <= other.y); }				// Compare if this vector is less than or equal to another vector (component-wise)
		inline bool operator>=(const Vector2 other) const				{ return (x >= other.x && y >= other.y); }				// Compare if this vector is greater than or equal to another vector (component-wise)

		inline float operator[](const size_t index) const				{ assert(index < 2); return *(&x + index); }			// Access vector components by index (0 for x, 1 for y)
		inline float& operator[](const size_t index)					{ assert(index < 2); return *(&x + index); }			// Access vector components by index (0 for x, 1 for y)

		inline Vector2 operator-() const								{ return { -x, -y }; }									// Negate the vector (unary minus)
		inline Vector2 operator+(const Vector2 other) const				{ return { x + other.x, y + other.y }; }				// Add two vectors component-wise
		inline Vector2 operator-(const Vector2 other) const				{ return { x - other.x, y - other.y }; }				// Subtract two vectors component-wise
		inline Vector2 operator*(const Vector2 other) const				{ return { x * other.x, y * other.y }; }				// Multiply two vectors component-wise
		inline Vector2 operator/(const Vector2 other) const				{ return { x / other.x, y / other.y }; }				// Divide two vectors component-wise

		inline Vector2& operator+=(const Vector2 other)					{ return *this = *this + other; }						// Add another vector to this vector component-wise and assign the result to this vector
		inline Vector2& operator-=(const Vector2 other)					{ return *this = *this - other; }						// Subtract another vector from this vector component-wise and assign the result to this vector
		inline Vector2& operator*=(const Vector2 other)					{ return *this = *this * other; }						// Multiply this vector by another vector component-wise and assign the result to this vector
		inline Vector2& operator/=(const Vector2 other)					{ return *this = *this / other; }						// Divide this vector by another vector component-wise and assign the result to this vector

		inline Vector2& operator+=(const float scalar)					{ return *this = *this + scalar; }						// Add a scalar to each component of this vector and assign the result to this vector
		inline Vector2& operator-=(const float scalar)					{ return *this = *this - scalar; }						// Subtract a scalar from each component of this vector and assign the result to this vector
		inline Vector2& operator*=(const float scalar)					{ return *this = *this * scalar; }						// Multiply each component of this vector by a scalar and assign the result to this vector
		inline Vector2& operator/=(const float scalar)					{ return *this = *this / scalar; }						// Divide each component of this vector by a scalar and assign the result to this vector

		static constexpr inline Vector2 Up()							{ return { 0, 1 }; }									// Returns a vector pointing upwards (0, 1)
		static constexpr inline Vector2 Down()							{ return { 0, -1 }; }									// Returns a vector pointing downwards (0, -1)
		static constexpr inline Vector2 Left()							{ return { -1, 0 }; }									// Returns a vector pointing leftwards (-1, 0)
		static constexpr inline Vector2 Right()							{ return { 1, 0 }; }									// Returns a vector pointing rightwards (1, 0)
		static constexpr inline Vector2 One()							{ return { 1, 1 }; }									// Returns a vector with all components set to 1 (1, 1)
		static constexpr inline Vector2 Zero()							{ return { 0, 0 }; }									// Returns a vector with all components set to 0 (0, 0)

		inline T X() const												{ return x; }											// Get the x component of the vector
		inline T Y() const												{ return y; }											// Get the y component of the vector

		/*-----------------------------------------------------
		| --- Magnitude: Returns the length of the vector --- |
		-----------------------------------------------------*/
		inline T Magnitude() const
		{
			return std::sqrt(SqrMagnitude());
		}

		/*----------------------------------------------------------------
		| --- SqrMagnitude: Returns the squared length of the vector --- |
		----------------------------------------------------------------*/
		inline T SqrMagnitude() const
		{
			return x * x + y * y;
		}

		/*-----------------------------------------------------------------------------------
		| --- Normalized: Returns a vector with the same direction but a magnitude of 1 --- |
		-----------------------------------------------------------------------------------*/
		inline Vector2 Normalized() const
		{
			return *this / Magnitude();
		}

		/*------------------------------------------------------------------------
		| --- Equals: Returns whether this vector is equal to another vector --- |
		------------------------------------------------------------------------*/
		inline bool Equals(const Vector2& other) const
		{
			return *this == other;
		}

		/*--------------------------------------------------------------------------------
		| --- Set: Sets the x and y components of the vector to the specified values --- |
		--------------------------------------------------------------------------------*/
		inline void Set(const T newX, const T newY)
		{
			x = newX;
			y = newY;
		}

		/*----------------------------------------------------------------------------------------
		| --- ToString: Returns a string representation of the vector in the format "{x, y}" --- |
		----------------------------------------------------------------------------------------*/
		inline std::string ToString() const
		{
			return "{" + std::to_string(x) + ", " + std::to_string(y) + "}";
		}

		/*--------------------------------------------------------------------------
		| --- Angle: Returns the unsigned angle in degrees between from and to --- |
		--------------------------------------------------------------------------*/
		static inline T Angle(const Vector2& from, const Vector2& to)
		{
			return std::acos(Dot(from, to) / (from.Magnitude() * to.Magnitude())) * (180.0f / 3.14159265358979323846f);
		}

		/*------------------------------------------------------------------------------------------
		| --- ClampMagnitude: Returns a copy of vector with its magnitude clamped to maxLength --- |
		------------------------------------------------------------------------------------------*/
		static inline Vector2 ClampMagnitude(const Vector2& vector, const float maxLength)
		{
			if (vector.Magnitude() > maxLength)
			{
				return vector.Normalized() * maxLength;
			}
			return vector;
		}

		/*--------------------------------------------------------
		| --- Distance: Returns the distance between a and b --- |
		--------------------------------------------------------*/
		static inline T Distance(const Vector2& a, const Vector2& b)
		{
			return (a - b).Magnitude();
		}

		/*-------------------------------------------------------------------
		| --- SqrDistance: Returns the squared distance between a and b --- |
		-------------------------------------------------------------------*/
		static inline T SqrDistance(const Vector2& a, const Vector2& b)
		{
			return (a - b).SqrMagnitude();
		}

		/*-----------------------------------------------------
		| --- Dot: Returns the dot product of two vectors --- |
		-----------------------------------------------------*/
		static inline T Dot(const Vector2& lhs, const Vector2& rhs)
		{
			return lhs.x * rhs.x + lhs.y * rhs.y;
		}

		/*----------------------------------------------------------
		| --- Lerp: Linearly interpolates between a and b by t --- |
		----------------------------------------------------------*/
		static inline Vector2 Lerp(const Vector2& a, const Vector2& b, const float t)
		{
			float clampedT = std::clamp(t, 0.0f, 1.0f);
			return a + (b - a) * clampedT;
		}
	
		/*----------------------------------------------------------------------------------------------------
		| --- LerpUnclamped: Linearly interpolates between a and b by t without clamping the interpolant --- |
		----------------------------------------------------------------------------------------------------*/
		static inline Vector2 LerpUnclamped(const Vector2& a, const Vector2& b, const float t)
		{
			return a + (b - a) * t;
		}

		// static methods
		// Angle(Vec2 from, Vec2 to)															- Returns the unsigned angle in degrees between from and to
		// ClampMagnitude(Vec2 vector, float maxLength)											- Returns a copy of vector with its magnitude clamped to maxLength
		// Distance(Vec2 a, Vec2 b)																- Returns the distance between a and b
		// Dot(Vec2 lhs, Vec2 rhs)																- Returns the dot product of two vectors
		// Lerp(Vec2 a, Vec2 b, float t)														- Linearly interpolates between a and b by t
		// LerpUnclamped(Vec2 a, Vec2 b, float t)												- Linearly interpolates between a and b by t without clamping the interpolant
		// Max(Vec2 lhs, Vec2 rhs)																- Returns a vector that is made from the largest components of two vectors
		// Min(Vec2 lhs, Vec2 rhs)																- Returns a vector that is made from the smallest components of two vectors
		// MoveTowards(Vec2 current, Vec2 target, float maxDistanceDelta)						- Moves a point current towards target
		// Normalize																			- Makes this vector have a magnitude of 1
		// Perpendicular(Vec2 inDirection)														- Returns a 2D vector perpendicular to this 2D vector
		// Reflect(Vec2 inDirection, Vec2 inNormal)												- Reflects a vector off the plane defined by a normal
		// Scale(Vec2 a, Vec2 b)																- Multiplies two vectors component-wise
		// SignedAngle(Vec2 from, Vec2 to)														- Returns the signed angle in degrees between from and to
		// 
		// SmoothDamp(Vec2 current, Vec2 target, ref Vec2 currentVelocity, float smoothTime,
		// float maxSpeed = Mathf.Infinity, float deltaTime = Time.deltaTime)					- Gradually changes a vector towards a desired goal over time
	};

	// Type aliases for common vector types
	using Vector2i = Vector2<int>;
	using Vector2f = Vector2<float>;
	using Vector2d = Vector2<double>;
}