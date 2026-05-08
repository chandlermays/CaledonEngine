#pragma once
#include <string>

namespace CE
{
	class Image
	{
	public:
		virtual ~Image() = default;					// Destructor

		virtual int GetW() const = 0;				// Get the width of the image
		virtual int GetH() const = 0;				// Get the height of the image
	};
}