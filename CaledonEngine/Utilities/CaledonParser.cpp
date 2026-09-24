/*------------------------------
| File: CaledonParser.cpp
| Author: Chandler Mays
------------------------------*/
#include "CaledonParser.h"

#include "Systems/Engine/LoggingManager.h"
#include "Utilities/ThirdParty/tinyxml2.h"

#include <fstream>
#include <sstream>

/*-----------------------------------
| --- Public Method Definitions --- |
-----------------------------------*/
/*-----------------------------------------------------------------------
| --- Constructor: Constructs the CaledonParser with default values --- |
-----------------------------------------------------------------------*/
CE::CaledonParser::CaledonParser()
    : m_pDocument{ std::make_unique<tinyxml2::XMLDocument>() }
{ }

CE::CaledonParser::~CaledonParser() = default;		// Here, where tinyxml2.h makes XMLDocument complete

/*-------------------------------------------------------------------------
| --- LoadFile: 
-------------------------------------------------------------------------*/
bool CE::CaledonParser::LoadFile(const std::string& filePath)
{
    m_pDocument->Clear();

    tinyxml2::XMLError result = m_pDocument->LoadFile(filePath.c_str());

    if (result != tinyxml2::XML_SUCCESS)
    {
        CE_LOG("Error loading the XML file: " + filePath);
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------
| --- Parse:
-------------------------------------------------------------------------*/
bool CE::CaledonParser::Parse(const std::string& fileData)
{
    m_pDocument->Clear();

    tinyxml2::XMLError result = m_pDocument->Parse(fileData.c_str());

    if (result != tinyxml2::XML_SUCCESS)
    {
        CE_LOG("Error parsing the XML data: " + std::to_string(result));
        return false;
    }
    return true;
}

/*-------------------------------------------------------------------------
| --- GetRootElement:
-------------------------------------------------------------------------*/
tinyxml2::XMLElement* CE::CaledonParser::GetRootElement(const std::string& rootName)
{
    return m_pDocument->FirstChildElement(rootName.c_str());
}

/*-------------------------------------------------------------------------
| --- ElementToString:
-------------------------------------------------------------------------*/
std::string CE::CaledonParser::ElementToString(tinyxml2::XMLElement* pElement)
{
    if (!pElement)
    {
        return "";
    }

    tinyxml2::XMLPrinter printer;
    pElement->Accept(&printer);
    return printer.CStr();
}

/*-------------------------------------------------------------------------
| --- ExtractStructure:
-------------------------------------------------------------------------*/
std::unordered_map<std::string, std::string> CE::CaledonParser::ExtractStructure(const std::string& root, const std::string& element, const std::string& keyName, const std::string& valueName)
{
    std::unordered_map<std::string, std::string> keyValuePairs;
    tinyxml2::XMLElement* pRoot = GetRootElement(root);

    if (pRoot)
    {
        for (tinyxml2::XMLElement* pElement = pRoot->FirstChildElement(element.c_str()); pElement != nullptr; pElement = pElement->NextSiblingElement(element.c_str()))
        {
            const char* key = pElement->Attribute(keyName.c_str());
            const char* value = pElement->Attribute(valueName.c_str());

            if (key && value)
            {
                keyValuePairs[key] = value;
            }
        }
    }

    return keyValuePairs;
}