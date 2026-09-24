/*------------------------------
| File: CaledonParser.h
| Author: Chandler Mays
------------------------------*/
#pragma once
#include <memory>
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
		std::unique_ptr<tinyxml2::XMLDocument> m_pDocument;														// 

	public:
		CaledonParser();																						// Constructor
		~CaledonParser();																						// Destructor
		CaledonParser(const CaledonParser&) = delete;															// Prevent copy-construction
		CaledonParser& operator=(const CaledonParser&) = delete;												// Prevent copy-assignment
		CaledonParser(CaledonParser&&) = delete;																// Prevent move-construction
		CaledonParser& operator=(CaledonParser&&) = delete;														// Prevent move-assignment

		bool LoadFile(const std::string& filePath);
		bool Parse(const std::string& fileData);

		tinyxml2::XMLElement* GetRootElement(const std::string& rootName);
		std::string ElementToString(tinyxml2::XMLElement* pElement);

		std::unordered_map<std::string, std::string> ExtractStructure(const std::string& root,
			const std::string& element, const std::string& key, const std::string& value);

		tinyxml2::XMLDocument* GetDocument() const { return m_pDocument.get(); }
	};
}