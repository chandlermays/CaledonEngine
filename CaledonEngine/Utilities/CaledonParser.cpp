#include "CaledonParser.h"
#include "CaledonEngine/Systems/Engine/LoggingManager.h"
#include "CaledonEngine/Utilities/ThirdParty/tinyxml2.h"

#include <fstream>
#include <sstream>

CE::CaledonParser::CaledonParser()
    : m_document{ new tinyxml2::XMLDocument() }
{
}

CE::CaledonParser::~CaledonParser()
{
    delete m_document;
    m_document = nullptr;
}

bool CE::CaledonParser::LoadFile(const std::string& filePath)
{
    tinyxml2::XMLError result = m_document->LoadFile(filePath.c_str());

    if (result != tinyxml2::XML_SUCCESS)
    {
        CE_LOG("Error loading the XML file: " + filePath);
        return false;
    }
    return true;
}

bool CE::CaledonParser::Parse(const std::string& fileData)
{
    tinyxml2::XMLError result = m_document->Parse(fileData.c_str());

    if (result != tinyxml2::XML_SUCCESS)
    {
        CE_LOG("Error parsing the XML data: " + std::to_string(result));
        return false;
    }
    return true;
}

tinyxml2::XMLElement* CE::CaledonParser::GetRootElement(const std::string& rootName)
{
    return m_document->FirstChildElement(rootName.c_str());
}

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