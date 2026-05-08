#pragma once
#include <string>
#include <unordered_map>

namespace tinyxml2
{
	class XMLElement;
	class XMLDocument;
	class XMLPrinter;
}

namespace CE
{
	class CaledonParser
	{
	private:
		tinyxml2::XMLDocument* m_document;

	public:
		CaledonParser();
		~CaledonParser();

		bool LoadFile(const std::string& filePath);
		bool Parse(const std::string& fileData);
		tinyxml2::XMLElement* GetRootElement(const std::string& rootName);
		std::string ElementToString(tinyxml2::XMLElement* pElement);

		std::unordered_map<std::string, std::string> ExtractStructure(const std::string& root,
			const std::string& element, const std::string& key, const std::string& value);

		tinyxml2::XMLDocument* GetDocument() const { return m_document; }
	};
}