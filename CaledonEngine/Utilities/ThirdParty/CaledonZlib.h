/*------------------------------
| File: CaledonZlib.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <stdint.h>
#include <string>
#include <map>

namespace CE
{
	class CaledonZLib
	{
	private:
		std::map<std::string, std::string> m_uncompressedData;					// Map to store uncompressed data with file names as keys

	public:
		CaledonZLib();															// Constructor
		~CaledonZLib() = default;												// Destructor

		std::string GetUncompressedData(const std::string& filePath) const;		// Get the Data from the Associated File
	};
}