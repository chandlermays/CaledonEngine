#pragma once
#include <stdint.h>
#include <string>
#include <map>

namespace CE
{
	class CaledonZLib
	{
	private:
		std::map<std::string, std::string> m_uncompressedData;

	public:
		CaledonZLib();
		~CaledonZLib() = default;

		std::string GetUncompressedData(const std::string& filePath) const;		// Get the Data from the Associated File
	};
}